#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Machine {
  size_t num_lights;
  uint16_t light_spec;  // bitmap. Max is 10 lights in input.
  uint16_t buttons[16]; // Input data has fewer than 16 buttons.
  size_t num_buttons;
};

void skip_spaces(const char **ptr) {
  while (**ptr == ' ') {
    (*ptr)++;
  }
}

void consume_char(const char **ptr, char c) {
  if (**ptr != c) {
    fprintf(stderr, "Expected '%c' but got '%c'\n", c, **ptr);
    exit(EXIT_FAILURE);
  }
  (*ptr)++;
}

uint16_t consume_next_button(const char **ptr) {
  skip_spaces(ptr);
  consume_char(ptr, '(');

  uint16_t button = 0;
  while (**ptr != ')') {
    char *endptr;
    uint16_t light_num = strtol(*ptr, &endptr, 10);
    if (endptr == *ptr) {
      fprintf(stderr, "Expected a number");
      exit(EXIT_FAILURE);
    }
    *ptr = endptr;
    button += (1 << light_num);
    if (**ptr == ',') {
      (*ptr)++;
    }
  }
  consume_char(ptr, ')');
  return button;
}

void parse_machine(const char *line, struct Machine *m) {
  printf("%s", line);
  size_t len = strlen(line);

  uint16_t light_spec = 0;
  size_t i = 0;
  while (line[i] != ']') {
    light_spec >>= 1;
    if (line[i] == '#') {
      light_spec += 1 << 15;
    }
    ++i;
  }
  m->light_spec = light_spec >> (17 - i);
  m->num_lights = i - 2;
  m->num_buttons = 0;

  const char *ptr = &line[i] + 1;
  skip_spaces(&ptr);
  while (*ptr == '(') {
    uint16_t btn = consume_next_button(&ptr);
    printf("Got button %d\n", btn);
    skip_spaces(&ptr);
    m->buttons[m->num_buttons++] = btn;
  }
}

uint64_t fewest_presses(struct Machine *m) { return 1; }

int main() {
  const char *input_path = "input/samples/day10_1.txt";
  // const char *input_path = "input/real/day10_1.txt";

  FILE *f = fopen(input_path, "r");
  if (!f) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  uint64_t sum = 0;
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    struct Machine *m = (struct Machine *)malloc(sizeof(struct Machine));
    parse_machine(line, m);
    printf("%d\n", m->light_spec);
    sum += fewest_presses(m);
    free(m);
  }

  printf("Part 1: %" PRIu64 "\n", sum); // 964
  // printf("Part 2: %d\n", part2_count); // 5872

  fclose(f);
  return EXIT_SUCCESS;
}
