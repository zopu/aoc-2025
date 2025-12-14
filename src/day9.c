#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
  uint32_t x;
  uint32_t y;
} Point2D;

Point2D parse_line(const char *line) {
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

  return (Point2D){.x = x, .y = y};
}

uint64_t box_area(Point2D a, Point2D b) {
  int32_t dx = abs((int32_t)a.x - (int32_t)b.x) + 1;
  int32_t dy = abs((int32_t)a.y - (int32_t)b.y) + 1;
  return (uint64_t)dx * dy;
}
void part1(Point2D *points, size_t point_count) {
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

bool point_in_poly_f(Point2D *points, size_t point_count, float x, float y,
                     bool log) {
  // cast a ray in x direction and count intersections. If odd, inside.
  size_t intersections = 0;
  for (size_t pi = 0; pi < point_count; pi++) {
    Point2D a = points[pi];
    Point2D b = points[(pi + 1) % point_count];
    if (a.x != b.x) {
      // only check orthogonal edges
      continue;
    }
    if ((a.y > y && b.y > y) || (a.y < y && b.y < y)) {
      // Line segment is entirely above or below point
      continue;
    }
    if (fabs((float)a.y - y) < 1e-6) {
      // y is touching the end of this line segment, so don't count.
      continue;
    }
    if (fabs((float)b.y - y) < 1e-6) {
      // y is touching the end of this line segment, so don't count.
      continue;
    }
    if (a.x <= x) {
      // Line segment is entirely to the left of point
      continue;
    }
    intersections++;
  }
  if (intersections % 2 == 0) {
    return false;
  }
  return true;
}

bool point_in_poly_int(Point2D *points, size_t point_count, uint32_t x,
                       uint32_t y, bool log) {
  // cast a ray in x direction and count intersections. If odd, inside.
  size_t intersections = 0;
  for (size_t pi = 0; pi < point_count; pi++) {
    Point2D a = points[pi];
    Point2D b = points[(pi + 1) % point_count];
    if (a.x != b.x) {
      // only check orthogonal edges
      continue;
    }
    if ((a.y > y && b.y > y) || (a.y < y && b.y < y)) {
      // Line segment is entirely above or below point
      continue;
    }
    if (a.y == y) {
      // y is touching the end of this line segment, so don't count.
      continue;
    }
    if (b.y == y) {
      // y is touching the end of this line segment, so don't count.
      continue;
    }
    if (a.x <= x) { // Line segment is entirely to the left of point
      continue;
    }
    // printf("Edge (%u,%u)-(%u,%u) intersects with y=%f\n", a.x, a.y, b.x, b.y,
    //        y);
    intersections++;
  }
  if (intersections % 2 == 0) {
    return false;
  }
  return true;
}

bool rect_in_poly(Point2D *points, size_t point_count, Point2D a, Point2D b,
                  bool log) {
  uint32_t rect_x_bounds[2] = {
      MIN(a.x, b.x), // min x
      MAX(a.x, b.x), // max x
  };
  uint32_t rect_y_bounds[2] = {
      MIN(a.y, b.y), // min y
      MAX(a.y, b.y)  // max y
  };
  Point2D middle = {.x = (a.x + b.x) / 2, .y = (a.y + b.y) / 2};
  if (!point_in_poly_f(points, point_count, (float)(a.x + b.x) / 2.0,
                       (float)(a.y + b.y) / 2.0, log)) {
    return false;
  }
  if (!point_in_poly_int(points, point_count, (float)rect_x_bounds[0],
                         (float)rect_y_bounds[0], log)) {
    return false;
  }
  if (!point_in_poly_int(points, point_count, (float)rect_x_bounds[0],
                         (float)rect_y_bounds[1], log)) {
    return false;
  }
  if (!point_in_poly_int(points, point_count, (float)rect_x_bounds[1],
                         (float)rect_y_bounds[0], log)) {
    return false;
  }
  if (!point_in_poly_int(points, point_count, (float)rect_x_bounds[1],
                         (float)rect_y_bounds[1], log)) {
    return false;
  }

  // Now for every edge check that it doesn't intersect with any orthogonal poly
  for (size_t pei = 0; pei < point_count; pei++) {
    Point2D pa = points[pei];
    Point2D pb = points[(pei + 1) % point_count];
    uint32_t edge_x_bounds[2] = {
        MIN(pa.x, pb.x), // min x
        MAX(pa.x, pb.x), // max x
    };
    uint32_t edge_y_bounds[2] = {
        MIN(pa.y, pb.y), // min y
        MAX(pa.y, pb.y)  // max y
    };
    bool vert_edge = (pa.x == pb.x);
    bool horiz_edge = (pa.y == pb.y);

    // Rect edge with min x
    bool edge_x_1_intersects = vert_edge && pa.x > rect_x_bounds[0] &&
                               pa.x < rect_x_bounds[1] &&
                               edge_y_bounds[0] < rect_y_bounds[0] &&
                               edge_y_bounds[1] > rect_y_bounds[0];

    // Rect edge with max x
    bool edge_x_2_intersects = vert_edge && pa.x > rect_x_bounds[0] &&
                               pa.x < rect_x_bounds[1] &&
                               edge_y_bounds[0] < rect_y_bounds[1] &&
                               edge_y_bounds[1] > rect_y_bounds[1];

    // Rect edge with min y
    bool edge_y_1_intersects = horiz_edge && pa.y > rect_y_bounds[0] &&
                               pa.y < rect_y_bounds[1] &&
                               edge_x_bounds[0] < rect_x_bounds[0] &&
                               edge_x_bounds[1] > rect_x_bounds[0];

    // Rect edge with max y
    bool edge_y_2_intersects = horiz_edge && pa.y > rect_y_bounds[0] &&
                               pa.y < rect_y_bounds[1] &&
                               edge_x_bounds[0] < rect_x_bounds[1] &&
                               edge_x_bounds[1] > rect_x_bounds[1];
    if (edge_x_1_intersects || edge_x_2_intersects || edge_y_1_intersects ||
        edge_y_2_intersects) {
      return false;
    }

    // Now check if the edge is entirely contained within the rectangle
    // We can't re-use the intersects logic for these as they don't detect edges
    // with a vertex on the rectangle edge.

    if (vert_edge) {
      // Check if vertical edge is entirely within rectangle
      if (pa.x > rect_x_bounds[0] && pa.x < rect_x_bounds[1] &&
          edge_y_bounds[0] >= rect_y_bounds[0] &&
          edge_y_bounds[1] <= rect_y_bounds[1]) {
        return false;
      }
    } else {
      // horizontal edge
      if (pa.y > rect_y_bounds[0] && pa.y < rect_y_bounds[1] &&
          edge_x_bounds[0] >= rect_x_bounds[0] &&
          edge_x_bounds[1] <= rect_x_bounds[1]) {
        return false;
      }
    }
  }
  return true;
}

void part2(Point2D *points, size_t point_count) {
  // n^2 to start with
  uint64_t max_box_area = 0;
  size_t max_idx_a = 0;
  size_t max_idx_b = 1;
  for (size_t i = 0; i < point_count - 1; i++) {
    for (size_t j = i + 1; j < point_count; j++) {
      uint64_t ba = box_area(points[i], points[j]);
      if (ba > max_box_area &&
          rect_in_poly(points, point_count, points[i], points[j], false)) {
        max_box_area = ba;
        max_idx_a = i;
        max_idx_b = j;
      }
    }
  }
  printf("Max at (%u,%u),(%u,%u)\n", points[max_idx_a].x, points[max_idx_a].y,
         points[max_idx_b].x, points[max_idx_b].y);
  printf("Part 2: %llu\n", max_box_area); // 1529011204
}

int main() {
  const char *input_path = "input/samples/day9_1.txt";
  // const char *input_path = "input/real/day9_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  Point2D points[500];
  size_t point_count = 0;

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    points[point_count++] = parse_line(line);
  }

  part1(points, point_count);
  part2(points, point_count);

  return EXIT_SUCCESS;
}
