#include "prompt_line.h"

#include <stdio.h>

int main() {
  char *str = NULL;
  size_t len = prompt_line(stdin, stdout, "enter string: ", &str);

  printf("str: %s\n", (str ? str : ""));
  printf("len: %lu\n", len);

  return 0;
}
