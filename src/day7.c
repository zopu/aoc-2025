#include <inttypes.h>
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

  char line[MAX_LINE_LENGTH];

  // First get the top line with the 'S' and use it to measure the line length.
  if (!fgets(line, sizeof(line), f)) {
    fclose(f);
    return EXIT_FAILURE;
  }
  size_t line_len = strlen(line) - 1; // Take off the \n
  size_t line_len_bytes = line_len * sizeof(uint64_t);
  memset(beams, 0, line_len_bytes);
  memset(beams_next, 0, line_len_bytes);
  char *s_ptr = strchr(line, 'S');
  if (!s_ptr) {
    fprintf(stderr, "No 'S' on first line!\n");
    fclose(f);
    return EXIT_FAILURE;
  }
  size_t s_loc = s_ptr - line;
  beams[s_loc] = 1;
  beams_next[s_loc] = 1;

  uint32_t split_count = 0;
  while (fgets(line, sizeof(line), f)) {
    for (size_t i = 0; line[i] != '\0'; ++i) {
      if (line[i] == '^') {
        beams_next[i] = 0;
        if (beams[i] > 0) {
          split_count++;
          if (i > 0)
            beams_next[i - 1] += beams[i];
          if (i + 1 < line_len)
            beams_next[i + 1] += beams[i];
        }
      }
    }
    memcpy(beams, beams_next, line_len_bytes);
  }

  uint64_t p2_sum = 0;
  for (size_t i = 0; i < line_len; ++i) {
    p2_sum += beams[i];
  }

  printf("Part 1: %" PRIu32 "\n", split_count); // 1507
  printf("Part 2: %" PRIu64 "\n", p2_sum);      // 1537373473728

  fclose(f);
  return EXIT_SUCCESS;
}
