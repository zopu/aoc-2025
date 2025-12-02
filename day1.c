#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DIAL_SIZE 100
#define DIAL_START 50

int32_t parse_line(const char *line) {
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

int32_t make_move(int32_t current_pos, int32_t move, uint32_t *crossings) {
  *crossings = abs(move / DIAL_SIZE);
  int32_t mov_modded = move % DIAL_SIZE;
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

  int32_t pos = DIAL_START;
  uint32_t part1_count = 0;
  uint32_t part2_count = 0;

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    int32_t move = parse_line(line);
    uint32_t crossings = 0;
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
