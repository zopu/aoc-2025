#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Point2D {
  uint32_t x;
  uint32_t y;
};

struct Point2D parse_line(const char *line) {
  char *endptr;
  long x = strtol(line, &endptr, 10);
  if (endptr == line) {
    fprintf(stderr, "Invalid x in line: %s\n", line);
    exit(EXIT_FAILURE);
  }
  char *ptr = endptr + 1; // Skip comma
  long y = strtol(ptr, &endptr, 10);
  if (endptr == ptr) {
    fprintf(stderr, "Invalid y in line: %s\n", line);
    exit(EXIT_FAILURE);
  }

  return (struct Point2D){.x = x, .y = y};
}

uint64_t box_area(struct Point2D a, struct Point2D b) {
  int32_t dx = abs((int32_t)a.x - (int32_t)b.x) + 1;
  int32_t dy = abs((int32_t)a.y - (int32_t)b.y) + 1;
  return (uint64_t)dx * dy;
}

void part1(struct Point2D *points, size_t point_count) {
  // n^2 to start with
  uint64_t max_box_area = 0;
  size_t max_idx_a = 0;
  size_t max_idx_b = 1;
  for (size_t i = 0; i < point_count - 1; i++) {
    for (size_t j = i + 1; j < point_count; j++) {
      uint64_t ba = box_area(points[i], points[j]);
      if (ba > max_box_area) {
        max_box_area = ba;
        max_idx_a = i;
        max_idx_b = j;
      }
    }
  }
  printf("Max at (%u,%u),(%u,%u)\n", points[max_idx_a].x, points[max_idx_a].y,
         points[max_idx_b].x, points[max_idx_b].y);
  printf("Part 1: %llu\n", max_box_area); // 4743645488
}

int main() {
  const char *input_path = "input/samples/day9_1.txt";
  // const char *input_path = "input/real/day9_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  struct Point2D points[500];
  size_t point_count = 0;

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    points[point_count++] = parse_line(line);
  }

  part1(points, point_count);

  return EXIT_SUCCESS;
}
