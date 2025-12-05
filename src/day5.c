#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Range {
  uint64_t start;
  uint64_t end;
};

int range_cmp_by_start(const void *a, const void *b) {
  const struct Range *ra = (const struct Range *)a;
  const struct Range *rb = (const struct Range *)b;
  if (ra->start < rb->start) {
    return -1;
  }
  if (ra->start > rb->start) {
    return 1;
  }
  return 0;
}

struct Range parse_range_line(const char *line) {
  char *endptr;
  unsigned long long range_start = strtoull(line, &endptr, 10);
  if (endptr == line) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }
  char *dash_ptr = endptr;
  unsigned long long range_end = strtoull(dash_ptr + 1, &endptr, 10);
  if (endptr == dash_ptr + 1) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }
  return (struct Range){range_start, range_end};
}

uint64_t parse_num_line(const char *line) {
  char *endptr;
  unsigned long long num = strtoull(line, &endptr, 10);
  if (endptr == line) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }
  return num;
}

int main() {
  // const char *input_path = "input/samples/day5_1.txt";
  const char *input_path = "input/real/day5_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  char line[256];
  bool parsing_ranges = true;
  struct Range ranges[1024];
  size_t range_count = 0;
  uint32_t part1_count = 0;
  while (fgets(line, sizeof(line), f)) {
    // TODO
    if (line[0] == '\n') {
      parsing_ranges = false;
      continue;
    }
    if (parsing_ranges) {
      struct Range r = parse_range_line(line);
      ranges[range_count++] = r;
    } else {
      uint64_t num = parse_num_line(line);
      for (size_t i = 0; i < range_count; i++) {
        struct Range r = ranges[i];
        if (num >= r.start && num <= r.end) {
          part1_count++;
          break;
        }
      }
    }
  }

  printf("Part 1: %d\n", part1_count); // 517

  qsort(ranges, range_count, sizeof(struct Range), range_cmp_by_start);

  // Dedup overlapping ranges
  for (size_t i = 0; i < range_count - 1;) {
    struct Range r1 = ranges[i];
    struct Range r2 = ranges[i + 1];
    if (r1.end >= r2.start) {
      // Overlapping ranges, merge them
      ranges[i].end = r1.end > r2.end ? r1.end : r2.end;
      // Shift remaining ranges left
      for (size_t j = i + 1; j < range_count - 1; j++) {
        ranges[j] = ranges[j + 1];
      }
      range_count--;
    } else {
      i++;
    }
  }

  uint64_t part2_count = 0;
  for (size_t i = 0; i < range_count; i++) {
    part2_count += ranges[i].end - ranges[i].start + 1;
  }
  printf("Part 2: %llu\n", part2_count); // 336173027056994

  fclose(f);
  return EXIT_SUCCESS;
}
