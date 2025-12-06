#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS "0123456789"
#define OPERATORS "+*"
#define MAX_COLUMNS 4000
#define MAX_NUMS_PER_LINE 4000

struct Tallys {
  uint64_t sums[MAX_NUMS_PER_LINE];
  uint64_t products[MAX_NUMS_PER_LINE];
  uint64_t column_nums[MAX_COLUMNS];
};

struct Tallys new_tallys() {
  struct Tallys t;
  for (size_t i = 0; i < MAX_NUMS_PER_LINE; i++) {
    t.sums[i] = 0;
    t.products[i] = 1;
  }
  for (size_t i = 0; i < MAX_COLUMNS; i++) {
    t.column_nums[i] = 0;
  }
  return t;
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

void process_op_line(const char *line, struct Tallys *tallys) {
  uint64_t p1_result = 0;
  uint64_t p2_result = 0;
  size_t count = 0;
  const char *ptr = line;
  char *op;
  while ((op = strpbrk(ptr, OPERATORS))) {
    ptr = op + 1;
    if (*op == '+') {
      p1_result += tallys->sums[count++];
      size_t i = ptr - line - 1;
      while (tallys->column_nums[i] != 0) {
        p2_result += tallys->column_nums[i];
        i++;
      }
    } else if (*op == '*') {
      p1_result += tallys->products[count++];
      size_t i = ptr - line - 1;
      uint64_t product = 1;
      while (tallys->column_nums[i] != 0) {
        product *= tallys->column_nums[i];
        i++;
      }
      p2_result += product;
    }
  }
  printf("Part 1: %llu\n", p1_result); // 6343365546996
  printf("Part 2: %llu\n", p2_result); // 11136895955912
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
  struct Tallys tallys = new_tallys();
  size_t max_line_length = 0;
  while (fgets(line, sizeof(line), f)) {
    if (strpbrk(line, OPERATORS)) {
      process_op_line(line, &tallys);
      break;
    }

    size_t len = strlen(line);
    if (len > max_line_length) {
      max_line_length = len;
    }
    update_column_nums(line, tallys.column_nums);
    uint32_t nums[MAX_NUMS_PER_LINE];
    char *ptr = line;
    size_t count = 0;
    while (strpbrk(ptr, DIGITS)) {
      nums[count++] = strtoul(ptr, &ptr, 10);
    }
    for (size_t i = 0; i < count; i++) {
      tallys.sums[i] += nums[i];
      tallys.products[i] *= nums[i];
    }
  }

  fclose(f);
  return EXIT_SUCCESS;
}
