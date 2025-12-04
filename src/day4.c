#include "common/grid.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *mmap_file(const char *path, size_t *out_size) {
  FILE *f = fopen(path, "r");
  if (!f) {
    perror("Failed to open file");
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  *out_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *data = malloc(*out_size);
  if (!data) {
    perror("Failed to allocate memory");
    fclose(f);
    return NULL;
  }

  fread(data, 1, *out_size, f);
  fclose(f);
  return data;
}

bool is_accessible(const struct GridView *grid, size_t x, size_t y) {
  // For each of the 8 neighbours, check if it is in bounds and contains '@'.
  // If there are 4 or more, return false.
  size_t cnt = 0;
  if (x > 0 && y > 0) { // Check top-left
    *grid_at(grid, x - 1, y - 1) == '@' ? cnt++ : 0;
  }
  if (y > 0) { // Check top
    *grid_at(grid, x, y - 1) == '@' ? cnt++ : 0;
  }
  if (x < grid->width - 1 && y > 0) { // Check top-right
    *grid_at(grid, x + 1, y - 1) == '@' ? cnt++ : 0;
  }
  if (x > 0) { // Check left
    *grid_at(grid, x - 1, y) == '@' ? cnt++ : 0;
  }
  if (x < grid->width - 1) { // Check right
    *grid_at(grid, x + 1, y) == '@' ? cnt++ : 0;
  }
  if (x > 0 && y < grid->height - 1) { // Check bottom-left
    *grid_at(grid, x - 1, y + 1) == '@' ? cnt++ : 0;
  }
  if (y < grid->height - 1) { // Check bottom
    *grid_at(grid, x, y + 1) == '@' ? cnt++ : 0;
  }
  if (x < grid->width - 1 && y < grid->height - 1) { // Check bottom-right
    *grid_at(grid, x + 1, y + 1) == '@' ? cnt++ : 0;
  }
  return cnt < 4;
}

int main() {
  // const char *input_path = "input/samples/day4_1.txt";
  const char *input_path = "input/real/day4_1.txt";

  size_t file_size;
  char *file_data = mmap_file(input_path, &file_size);
  if (!file_data) {
    return EXIT_FAILURE;
  }
  struct GridView *grid = create_grid(file_data, file_size);

  uint32_t part1_count = 0;
  for (int i = 0; i < grid->height; i++) {
    for (int j = 0; j < grid->width; j++) {
      char *cell = grid_at(grid, j, i);
      if (*cell == '@') {
        if (is_accessible(grid, j, i)) {
          part1_count++;
        }
      }
    }
  }

  uint32_t part2_count = 0;
  bool removed_some = true;
  while (removed_some) {
    removed_some = false;
    for (int i = 0; i < grid->height; i++) {
      for (int j = 0; j < grid->width; j++) {
        char *cell = grid_at(grid, j, i);
        if (*cell == '@') {
          if (is_accessible(grid, j, i)) {
            *cell = '.'; // Remove it
            part2_count++;
            removed_some = true;
          }
        }
      }
    }
  }

  printf("Part 1: %d\n", part1_count); // 1449
  printf("Part 2: %d\n", part2_count); // 8746

  free(grid);
  free(file_data);
  return EXIT_SUCCESS;
}
