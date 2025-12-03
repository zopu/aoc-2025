#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t max_embedded_num(const char *line, uint8_t digits) {
  if (digits == 0) {
    return 0;
  }
  // Scan and find the earliest position with the max digit
  // that's still digits places from the end.
  const char *ptr = line;

  char max_digit = 0;
  const char *max_digit_ptr = NULL;
  while (ptr[digits - 1] != '\0' && ptr[digits - 1] != '\n') {
    char digit = *ptr - '0';
    if (digit > max_digit) {
      max_digit = digit;
      max_digit_ptr = ptr;
    }
    ptr++;
  }
  return max_digit * pow(10, digits - 1) +
         max_embedded_num(max_digit_ptr + 1, digits - 1);
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
