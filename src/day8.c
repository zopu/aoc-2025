#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct Point3D {
  uint32_t x;
  uint32_t y;
  uint32_t z;
  uint32_t circuit;
};

static struct Point3D points[1024];

uint64_t distance_squared(struct Point3D *a, struct Point3D *b) {
  int64_t dx = (int64_t)a->x - (int64_t)b->x;
  int64_t dy = (int64_t)a->y - (int64_t)b->y;
  int64_t dz = (int64_t)a->z - (int64_t)b->z;
  return dx * dx + dy * dy + dz * dz;
}

struct IdxPair {
  size_t first;
  size_t second;
};

static struct IdxPair graph[1024];

bool graph_contains(struct IdxPair *pairs, size_t pair_count, size_t i,
                    size_t j) {
  for (size_t k = 0; k < pair_count; k++) {
    if ((pairs[k].first == i && pairs[k].second == j) ||
        (pairs[k].first == j && pairs[k].second == i)) {
      return true;
    }
  }
  return false;
}

int size_compare(const void *a, const void *b) {
  return (*(size_t *)b - *(size_t *)a);
}

struct IdxPair find_closest_unconnected(struct Point3D *points,
                                        size_t point_count,
                                        struct IdxPair *existing_pairs,
                                        size_t existing_pair_count) {
  size_t min_i = 0;
  size_t min_j = 0;
  uint64_t min_dist_sq = UINT64_MAX;

  for (size_t i = 0; i < point_count; i++) {
    for (size_t j = i + 1; j < point_count; j++) {
      uint32_t ci = points[i].circuit;
      uint32_t cj = points[j].circuit;
      if (ci == 0 || cj == 0 || ci != cj ||
          !graph_contains(existing_pairs, existing_pair_count, i, j)) {
        uint64_t d_sq = distance_squared(&points[i], &points[j]);
        if (d_sq < min_dist_sq) {
          min_dist_sq = d_sq;
          min_i = i;
          min_j = j;
        }
      }
    }
  }
  return (struct IdxPair){.first = min_i, .second = min_j};
}

struct IdxPair find_closest_unconnected_p2(struct Point3D *points,
                                           size_t point_count) {
  size_t min_i = 0;
  size_t min_j = 0;
  uint64_t min_dist_sq = UINT64_MAX;

  for (size_t i = 0; i < point_count; i++) {
    for (size_t j = i + 1; j < point_count; j++) {
      uint32_t ci = points[i].circuit;
      uint32_t cj = points[j].circuit;
      if (ci == 0 || cj == 0 || ci != cj) {
        uint64_t d_sq = distance_squared(&points[i], &points[j]);
        if (d_sq < min_dist_sq) {
          min_dist_sq = d_sq;
          min_i = i;
          min_j = j;
        }
      }
    }
  }
  return (struct IdxPair){.first = min_i, .second = min_j};
}

void merge_circuits(struct Point3D *points, size_t point_count,
                    struct Point3D *a, struct Point3D *b) {
  if (a->circuit == 0) {
    a->circuit = b->circuit;
    return;
  }
  if (b->circuit == 0) {
    b->circuit = a->circuit;
    return;
  }
  uint32_t cid = MIN(a->circuit, b->circuit);
  size_t ac = a->circuit;
  size_t bc = b->circuit;
  for (size_t i = 0; i < point_count; i++) {
    if (points[i].circuit == ac || points[i].circuit == bc) {
      points[i].circuit = cid;
    }
  }
}

struct Point3D parse_line(const char *line) {
  char *endptr;
  long x = strtol(line, &endptr, 10);
  if (endptr == line + 1) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }

  long y = strtol(endptr + 1, &endptr, 10);
  if (endptr == line + 1) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }

  long z = strtol(endptr + 1, &endptr, 10);
  if (endptr == line + 1) {
    fprintf(stderr, "Invalid input line: %s\n", line);
    exit(EXIT_FAILURE);
  }

  struct Point3D point = {.x = x, .y = y, .z = z};
  return point;
}

int main() {
  // const char *input_path = "input/samples/day8_1.txt";
  const char *input_path = "input/real/day8_1.txt";

  memset(points, 0, sizeof(points));

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  char line[256];
  size_t point_count = 0;
  while (fgets(line, sizeof(line), f)) {
    points[point_count++] = parse_line(line);
  }

  size_t num_connections = 1000;
  size_t next_circuit_id = 1;
  for (int i = 0; i < num_connections; i++) {
    struct IdxPair pair =
        find_closest_unconnected(points, point_count, graph, i);
    graph[i] = pair;
    if (points[pair.first].circuit == 0 && points[pair.second].circuit == 0) {
      points[pair.first].circuit = next_circuit_id;
      points[pair.second].circuit = next_circuit_id;
      next_circuit_id++;
    } else {
      merge_circuits(points, point_count, &points[pair.first],
                     &points[pair.second]);
    }
  }
  size_t circuit_counts[1024];
  memset(circuit_counts, 0, sizeof(circuit_counts));
  for (size_t i = 0; i < 1024; i++) {
    if (points[i].circuit > 0) {
      circuit_counts[points[i].circuit]++;
    }
  }

  qsort(circuit_counts, 1024, sizeof(size_t), size_compare);

  size_t p1_product = 1;
  for (int i = 0; i < 3; i++) {
    printf("Top circuit %d size: %zu\n", i, circuit_counts[i]);
    p1_product *= circuit_counts[i];
  }
  printf("Total circuits: %zu\n", next_circuit_id - 1);

  printf("Part 1: %zu\n", p1_product); // 42315

  while (true) {
    struct IdxPair pair = find_closest_unconnected_p2(points, point_count);
    if (points[pair.first].circuit == 0 && points[pair.second].circuit == 0) {
      points[pair.first].circuit = next_circuit_id;
      points[pair.second].circuit = next_circuit_id;
      next_circuit_id++;
    } else {
      merge_circuits(points, point_count, &points[pair.first],
                     &points[pair.second]);
    }
    // Check if all points are in the same circuit
    size_t circuit_id = points[0].circuit;
    if (circuit_id == 0) {
      continue;
    }
    bool all_connected = true;
    for (size_t j = 0; j < point_count; j++) {
      if (points[j].circuit != circuit_id) {
        all_connected = false;
        break;
      }
    }
    if (all_connected) {
      printf("All points connected\n");
      printf("Most recent connection: (%u,%u,%u) <-> (%u,%u,%u)\n",
             points[pair.first].x, points[pair.first].y, points[pair.first].z,
             points[pair.second].x, points[pair.second].y,
             points[pair.second].z);
      // 8079278220
      printf("Part 2: %llu\n",
             (uint64_t)points[pair.first].x * (uint64_t)points[pair.second].x);
      break;
    }
  }

  fclose(f);
  return EXIT_SUCCESS;
}
