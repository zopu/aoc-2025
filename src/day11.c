#include <_stdio.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define id_t uint32_t

struct Rack {
  id_t id;
  id_t *links;
  size_t num_links;
};

id_t parse_rack_id(const char *ptr) {
  return (ptr[0] << 16) + (ptr[1] << 8) + ptr[2];
}

struct Rack parse_line(const char *line) {
  id_t id = parse_rack_id(line);
  size_t len = strlen(line);
  size_t num_links = (len / 4) - 1;
  id_t *links = (id_t *)malloc(sizeof(id_t) * num_links);
  for (size_t i = 0; i < num_links; i++) {
    links[i] = parse_rack_id(&line[i * 4 + 5]);
  }
  return (struct Rack){id, links, num_links};
}

void print_rack(struct Rack *r) {
  printf("Rack id: %d\n", r->id);
  for (size_t i = 0; i < r->num_links; i++) {
    printf("  - link to %d\n", r->links[i]);
  }
}

void free_rack(struct Rack *r) {
  if (r == NULL) {
    return;
  }
  free(r->links);
}

size_t count_paths(struct Rack *racks, size_t rack_count, id_t start_id,
                   id_t end_id) {
  size_t path_count = 0;
  for (size_t i = 0; i < rack_count; i++) {
    if (racks[i].id == start_id) {
      for (size_t j = 0; j < racks[i].num_links; j++) {
        if (racks[i].links[j] == end_id) {
          path_count++;
        } else {
          path_count +=
              count_paths(racks, rack_count, racks[i].links[j], end_id);
        }
      }
      break;
    }
  }
  return path_count;
}

int main() {
  const char *input_path = "input/samples/day11_1.txt";
  // const char *input_path = "input/real/day11_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  id_t you = parse_rack_id("you");
  size_t you_idx = 0;
  id_t out = parse_rack_id("out");
  size_t out_idx = 0;

  struct Rack racks[1000];
  size_t num_racks = 0;

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    racks[num_racks] = parse_line(line);
    if (racks[num_racks].id == you) {
      you_idx = num_racks;
    }
    if (racks[num_racks].id == out) {
      out_idx = num_racks;
    }
    num_racks++;
  }

  for (size_t i = 0; i < num_racks; i++) {
    // print_rack(&racks[i]);
  }
  size_t path_count = count_paths(racks, num_racks, you, out);
  printf("Part 1: %zu\n", path_count); // 758

  fclose(f);
  return EXIT_SUCCESS;
}
