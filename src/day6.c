#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS "0123456789"
#define OPERATORS "+*"
#define MAX_COLUMNS 4000
#define MAX_NUMS_PER_LINE 1000

struct Tallys {
  uint64_t sums[MAX_NUMS_PER_LINE];
  uint64_t products[MAX_NUMS_PER_LINE];
  uint64_t column_nums[MAX_COLUMNS];
  size_t max_line_length;
};

static struct Tallys tallys;

static void tallys_init(struct Tallys *t) {
  memset(t, 0, sizeof(*t));
  for (size_t i = 0; i < MAX_NUMS_PER_LINE; i++) {
    t->products[i] = 1;
  }
}

static void update_column_nums(const char *restrict line,
                               uint64_t *restrict column_nums) {
  const char *ptr = line;
  size_t col = 0;
  while (*ptr != '\0') {
    if (*ptr >= '0' && *ptr <= '9') {
      column_nums[col] = column_nums[col] * 10 + (*ptr - '0');
    }
    ptr++;
    col++;
  }
}

static void process_num_line(const char *restrict line,
                             struct Tallys *restrict tallys) {
  size_t len = strlen(line);
  if (len > tallys->max_line_length) {
    tallys->max_line_length = len;
  }
  update_column_nums(line, tallys->column_nums);
  const char *ptr = line;
  size_t count = 0;
  while (strpbrk(ptr, DIGITS)) {
    char *endptr;
    uint32_t num = strtoul(ptr, &endptr, 10);
    ptr = endptr;
    tallys->products[count] *= num;
    tallys->sums[count] += num;
    count++;
  }
}

static void process_op_line(const char *restrict line,
                            struct Tallys *restrict tallys) {
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
  printf("Part 1: %" PRIu64 "\n", p1_result); // 6343365546996
  printf("Part 2: %" PRIu64 "\n", p2_result); // 11136895955912
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
  tallys_init(&tallys);
  while (fgets(line, sizeof(line), f)) {
    if (strpbrk(line, OPERATORS)) {
      process_op_line(line, &tallys);
      break;
    }
    process_num_line(line, &tallys);
  }
  fclose(f);
  return EXIT_SUCCESS;
}
