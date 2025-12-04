#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct GridView {
  size_t width;
  size_t height;
  char *cells;
};

struct GridView *create_grid(char *txt_file, size_t len);

void print_grid(const struct GridView *grid);

// x, y == column, row
char *grid_at(const struct GridView *grid, size_t x, size_t y);
