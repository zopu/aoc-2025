#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct GridView {
  size_t width;
  size_t height;
  char *cells;
};

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

struct GridView *create_grid(char *txt_file, size_t len) {
  struct GridView *grid = malloc(sizeof(struct GridView));
  if (!grid) {
    perror("Failed to allocate memory for grid");
    return NULL;
  }

  // Assume that each line is delimited and the same length
  const char *ptr = txt_file;
  while (*ptr != '\n' && *ptr != '\0') {
    grid->width++;
    ptr++;
  }
  grid->height = len / (grid->width + 1); // +1 for newline
  grid->cells = txt_file;

  return grid;
}

void print_grid(const struct GridView *grid) {
  for (size_t y = 0; y < grid->height; y++) {
    for (size_t x = 0; x < grid->width; x++) {
      putchar(grid->cells[y * (grid->width + 1) + x]);
    }
    putchar('\n');
  }
}

// x, y == column, row
char *grid_at(const struct GridView *grid, size_t x, size_t y) {
  if (x >= grid->width || y >= grid->height) {
    fprintf(stderr, "Grid access out of bounds: (%zu, %zu)\n", x, y);
    exit(EXIT_FAILURE);
  }
  return &grid->cells[y * (grid->width + 1) + x];
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
