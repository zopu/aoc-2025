#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMNS 4000
#define MAX_NUMS_PER_LINE 4000

bool line_has_more_nums(const char *line) {
  const char *ptr = line;
  while (*ptr != '\0') {
    if (*ptr >= '0' && *ptr <= '9') {
      return true;
    }
    ptr++;
  }
  return false;
}

bool read_next_operator(const char **line_ptr, char *operator) {
  const char *ptr = *line_ptr;
  while (*ptr != '\0') {
    if (*ptr == '+' || *ptr == '*') {
      *operator = *ptr;
      *line_ptr = ptr + 1;
      return true;
    }
    ptr++;
  }
  return false;
}

void update_column_nums(const char *line, uint64_t *column_nums) {
  const char *ptr = line;
  size_t col = 0;
  while (*ptr != '\0') {
    if (*ptr >= '0' && *ptr <= '9') {
      char digit = *ptr - '0';
      column_nums[col] = column_nums[col] * 10 + digit;
    }
    ptr++;
    col++;
  }
}

int main() {
  // const char *input_path = "input/samples/day6_1.txt";
  const char *input_path = "input/real/day6_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  char line[MAX_COLUMNS];
  uint64_t sums[MAX_NUMS_PER_LINE];
  uint64_t products[MAX_NUMS_PER_LINE];
  uint64_t part1_total = 0;
  for (size_t i = 0; i < MAX_NUMS_PER_LINE; i++) {
    sums[i] = 0;
    products[i] = 1;
  }
  uint64_t column_nums[MAX_COLUMNS];
  for (size_t i = 0; i < MAX_COLUMNS; i++) {
    column_nums[i] = 0;
  }
  uint64_t p1_result = 0;
  uint64_t p2_result = 0;
  size_t max_line_length = 0;
  while (fgets(line, sizeof(line), f)) {
    size_t len = strlen(line);
    if (len > max_line_length) {
      max_line_length = len;
    }
    size_t count = 0;
    if (!line_has_more_nums(line)) {
      // Compute the grand total
      const char *ptr = line;
      char operator;
      while (read_next_operator(&ptr, &operator)) {
        if (operator == '+') {
          p1_result += sums[count++];
          size_t i = ptr - line - 1;
          while (column_nums[i] != 0) {
            p2_result += column_nums[i];
            i++;
          }
        } else if (operator == '*') {
          p1_result += products[count++];
          size_t i = ptr - line - 1;
          uint64_t product = 1;
          while (column_nums[i] != 0) {
            product *= column_nums[i];
            i++;
          }
          p2_result += product;
        }
      }
      break;
    }
    update_column_nums(line, column_nums);
    uint32_t nums[MAX_NUMS_PER_LINE];
    char *ptr = line;
    while (line_has_more_nums(ptr)) {
      nums[count++] = strtoul(ptr, &ptr, 10);
    }
    for (size_t i = 0; i < count; i++) {
      sums[i] += nums[i];
      products[i] *= nums[i];
    }
  }

  printf("Part 1: %llu\n", p1_result); // 6343365546996
  printf("Part 2: %llu\n", p2_result); // 11136895955912

  fclose(f);
  return EXIT_SUCCESS;
}
