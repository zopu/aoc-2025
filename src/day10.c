#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
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
    skip_spaces(&ptr);
    m->buttons[m->num_buttons++] = btn;
  }
}

// In practice the search tends to come back to a relatively small set
// of states, so keep a small cache to stop repeating work.
#define SEEN_CACHE_SIZE 10000

struct SearchState {
  size_t count;
  uint16_t lights;
};

uint64_t fewest_presses(struct Machine *m) {
  uint16_t seen_cache[SEEN_CACHE_SIZE];
  memset(&seen_cache, 0, sizeof(seen_cache));
  size_t seen_cache_ptr = 0;

  struct SearchState *queue =
      (struct SearchState *)malloc(sizeof(struct SearchState) * 10000);
  size_t queue_cnt = 1;
  size_t queue_ptr = 0;

  queue[0] = (struct SearchState){.lights = 0, .count = 0};
  while (true) {
    uint16_t state = queue[queue_ptr].lights;
    size_t cnt = queue[queue_ptr].count;
    queue_ptr++;
    if (state == m->light_spec) {
      free(queue);
      return cnt;
    }
    for (int i = 0; i < m->num_buttons; i++) {
      struct SearchState ss = (struct SearchState){
          .lights = state ^ m->buttons[i], .count = cnt + 1};
      bool seen = false;
      for (int j = 0; j < seen_cache_ptr; j++) {
        if (seen_cache[j] == ss.lights) {
          seen = true;
        }
      }
      if (seen == true) {
        continue;
      }
      queue[queue_cnt++] = ss;
      if (seen_cache_ptr < SEEN_CACHE_SIZE) {
        seen_cache[seen_cache_ptr++] = ss.lights;
      }
    }
  }
  free(queue);
  return UINT64_MAX;
}

int main() {
  // const char *input_path = "input/samples/day10_1.txt";
  const char *input_path = "input/real/day10_1.txt";
  // const char *input_path = "input/tests/day10_1.txt";

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
    uint64_t fp = fewest_presses(m);
    sum += fp;
    free(m);
  }

  printf("Part 1: %" PRIu64 "\n", sum); // 964
  // printf("Part 2: %d\n", part2_count); // 5872

  fclose(f);
  return EXIT_SUCCESS;
}
