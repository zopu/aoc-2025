#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_next_pair(char **str, uint64_t *range_start, uint64_t *range_end) {
  if (*str == NULL || **str == '\0' || **str == '\n') {
    return EOF;
  }
  char *endptr;
  long rs = strtol(*str, &endptr, 10);
  if (endptr == *str) {
    fprintf(stderr, "Invalid input range: %s\n", *str);
    exit(EXIT_FAILURE);
  }
  *range_start = (uint64_t)rs;
  if (endptr == NULL || *endptr != '-') {
    fprintf(stderr, "Expected '-' after range start: %s\n", *str);
    exit(EXIT_FAILURE);
  }
  long re = strtol(endptr + 1, &endptr, 10);
  if (endptr == NULL || (endptr == *str)) {
    fprintf(stderr, "Invalid input range end: %s\n", *str);
    exit(EXIT_FAILURE);
  }

  *range_end = (uint64_t)re;

  if (*endptr == ',') {
    endptr++;
  }
  *str = endptr;
  return 0;
}

bool is_valid_id_p1(uint64_t id) {
  // It's invalid if it can be represented as x*10^n+x for n==num_digits(id)/2
  uint64_t n = 1;
  uint64_t x = id;
  while (x >= 10) {
    x /= 10;
    n++;
  }
  x = id;
  n /= 2;
  uint32_t tmp_n = n;
  while (tmp_n > 0) {
    x /= 10;
    tmp_n--;
  }
  return (id != (x * pow(10, n) + x));
}

bool is_valid_id_p2(uint64_t id) {
  // First represent the number as ASCII
  char buffer[50];
  sprintf(buffer, "%llu", id);

  // Check if the string is a repeating pattern (maybe many repeats)
  for (size_t len = strlen(buffer), i = 1; i <= len / 2; i++) {
    for (int skip = i; skip < len; skip += i) {
      if (strncmp(buffer, buffer + skip, i) != 0) {
        break;
      }
      if (skip + i >= len) {
        return false;
      }
    }
  }
  return true;
}

uint64_t sum_invalid_ids(uint64_t range_start, uint64_t range_end,
                         bool (*is_valid_id)(uint64_t)) {
  uint64_t sum = 0;
  for (uint64_t id = range_start; id <= range_end; id++) {
    if (!is_valid_id(id)) {
      sum += id;
    }
  }
  return sum;
}

int main() {
  // const char *input_path = "input/samples/day2_1.txt";
  const char *input_path = "input/real/day2_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  char line[1000];
  char *line_ptr = line;
  while (fgets(line, sizeof(line), f)) {
    uint64_t range_start = 0;
    uint64_t range_end = 0;
    uint64_t sum_invalids_p1 = 0;
    uint64_t sum_invalids_p2 = 0;

    while (parse_next_pair(&line_ptr, &range_start, &range_end) != EOF) {
      uint64_t invalid_count_p1 =
          sum_invalid_ids(range_start, range_end, is_valid_id_p1);
      sum_invalids_p1 += invalid_count_p1;

      uint64_t invalid_count_p2 =
          sum_invalid_ids(range_start, range_end, is_valid_id_p2);
      sum_invalids_p2 += invalid_count_p2;
    }
    printf("Part 1: %llu\n", sum_invalids_p1);
    printf("Part 2: %llu\n", sum_invalids_p2);
  }

  fclose(f);
  return EXIT_SUCCESS;
}
