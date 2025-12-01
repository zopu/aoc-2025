#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DIAL_SIZE 100
#define DIAL_START 50

int parse_line(const char *line) {
  char *endptr;
  long magnitude = strtol(line + 1, &endptr, 10);
  if (endptr == line + 1) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }
  if (magnitude > INT_MAX || magnitude < INT_MIN) {
    fprintf(stderr, "Magnitude out of range: %s\n", line);
    exit(EXIT_FAILURE);
  }
  return (line[0] == 'L') ? -magnitude : magnitude;
}

int make_move(int current_pos, int move, int *crossings) {
  *crossings = abs(move / DIAL_SIZE);
  int mov_modded = move % DIAL_SIZE;
  if (current_pos > 0 && current_pos + mov_modded < 0) {
    *crossings += 1;
  }
  if (current_pos + mov_modded > DIAL_SIZE) {
    *crossings += 1;
  }
  int new_pos = (current_pos + move) % DIAL_SIZE;
  if (new_pos < 0) {
    new_pos += DIAL_SIZE;
  }
  return new_pos;
}

int main() {
  // const char *input_path = "input/samples/day1_1.txt";
  const char *input_path = "input/real/day1_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  int pos = DIAL_START;
  int part1_count = 0;
  int part2_count = 0;

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    int move = parse_line(line);
    int crossings = 0;
    pos = make_move(pos, move, &crossings);
    if (pos == 0) {
      part1_count++;
      part2_count++;
    }
    part2_count += crossings;
  }

  printf("Part 1: %d\n", part1_count); // 964
  printf("Part 2: %d\n", part2_count); // 5872

  fclose(f);
  return EXIT_SUCCESS;
}
