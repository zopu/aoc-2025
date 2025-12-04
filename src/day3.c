#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t pow10(size_t n) {
  uint64_t result = 1;
  for (size_t i = 0; i < n; i++) {
    result *= 10;
  }
  return result;
}

static uint64_t max_embedded_num(const char *line, size_t digits) {
  if (digits == 0) {
    return 0;
  }
  const char *ptr = line;
  const char *max = ptr;
  while (ptr[digits - 1] != '\0' && ptr[digits - 1] != '\n') {
    if (*ptr > *max) {
      max = ptr;
    }
    ptr++;
  }
  return (*max - '0') * pow10(digits - 1) +
         max_embedded_num(max + 1, digits - 1);
}

int main() {
  // const char *input_path = "input/samples/day3_1.txt";
  const char *input_path = "input/real/day3_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  uint64_t part1_sum = 0;
  uint64_t part2_sum = 0;

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    part1_sum += max_embedded_num(line, 2);
    part2_sum += max_embedded_num(line, 12);
  }
  printf("Part 1: %llu\n", part1_sum); // 17100
  printf("Part 2: %llu\n", part2_sum); // 170418192256861

  fclose(f);
  return EXIT_SUCCESS;
}
