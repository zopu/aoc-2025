#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main() {
  const char *input_path = "input/samples/day7_1.txt";
  // const char *input_path = "input/real/day7_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  uint64_t beams[MAX_LINE_LENGTH];
  uint64_t beams_next[MAX_LINE_LENGTH];
  memset(beams, 0, sizeof(beams));
  memset(beams_next, 0, sizeof(beams));

  char line[MAX_LINE_LENGTH];
  fgets(line, sizeof(line), f);
  size_t i = 0;
  while (line[i] != '\0') {
    if (line[i] == 'S')
      beams[i] = 1;
    i++;
  }
  size_t line_len = i;

  uint32_t split_count = 0;
  while (fgets(line, sizeof(line), f)) {
    memset(beams_next, 0, sizeof(beams));
    for (size_t i = 0; line[i] != '\0'; ++i) {
      if (line[i] == '^') {
        beams_next[i] = 0;
        if (beams[i] > 0) {
          split_count++;
          if (i > 0)
            beams_next[i - 1] += beams[i];
          if (i < line_len)
            beams_next[i + 1] += beams[i];
        }
      } else if (beams[i] > 0) {
        beams_next[i] += beams[i];
      }
    }
    memcpy(beams, beams_next, sizeof(beams));
  }

  uint64_t p2_sum = 0;
  for (size_t i = 0; i < line_len; ++i) {
    p2_sum += beams[i];
  }

  printf("Part 1: %d\n", split_count); // 1507
  printf("Part 2: %llu\n", p2_sum);    // 1537373473728

  fclose(f);
  return EXIT_SUCCESS;
}
