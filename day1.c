#include <stdio.h>
#include <stdlib.h>

#define DIAL_SIZE 100
#define DIAL_START 50

int parse_line(const char *line) {
  int magnitude = atoi(line + 1);
  return (line[0] == 'L') ? -magnitude : magnitude;
}

int make_move(int current_pos, int move) {
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
    int small_move = move % DIAL_SIZE;
    int new_pos = make_move(pos, move);
    part2_count += abs(move / DIAL_SIZE);

    if ((pos > 0 && pos + small_move < 0) ||
        (pos < 0 && pos + small_move > 0)) {
      part2_count++;
    }
    if (pos + small_move > DIAL_SIZE) {
      part2_count++;
    }
    pos = new_pos;
    if (pos == 0) {
      part1_count++;
      part2_count++;
    }
  }

  printf("Part 1: %d\n", part1_count); // 964
  printf("Part 2: %d\n", part2_count); // 5872

  fclose(f);
  return EXIT_SUCCESS;
}
