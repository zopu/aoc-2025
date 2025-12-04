#include "grid.h"

struct GridView *create_grid(char *txt_file, size_t len) {
  struct GridView *grid = (struct GridView *)malloc(sizeof(struct GridView));
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
