// for prompt_line, PROMPT_LINE_MIN_SIZE, PROMPT_LINE_MAX_SIZE
#include "prompt_line.h"

// for ptrdiff_t, size_t
#include <stddef.h>
// for printf, stdin, stdout
#include <stdio.h>
// for realloc, free, FILE, ferror, feof, fread
#include <stdlib.h>
// for memchr, memset
#include <string.h>
// for true
#include <stdbool.h>

// similar to posix getline
size_t prompt_line(FILE *in, FILE *out, const char* prompt, char **const ret) {
  size_t size = PROMPT_LINE_MIN_SIZE;
  size_t len = 0;

  char *line = NULL;

  if (out && !ferror(out) && prompt) {
    fprintf(out, "%s", prompt);
  }

  while (true) {
    // realloc(..., 0) is undefined
    line = realloc(line, size + 1);
    if (!line) break;

    // read up to size
    size_t read_max = size - len;
    char *from = line + len;
    from = fgets(from, read_max + 1, in);
    if (!from || (ferror(in) && !feof(in))) break;

    // search for '\n' up to read_max
    char *const last = memchr(from, '\n', read_max);
    // last is null, or last > from
    size_t read_count = last ? (size_t)(last - from) : read_max;
    len += read_count;
    if (last) {
      // read_count <= read_max
      // zero the remaining bytes (at least one)
      memset(last, '\0', read_max - read_count + 1);
      *ret = line;
      return len;
    } else {
      // double the allocation for the next iteration, up to the limit
      size = (size < PROMPT_LINE_MAX_SIZE ? size * 2 : PROMPT_LINE_MAX_SIZE);
    }
  }
  if (line) {
    free(line);
  }
  *ret = NULL;
  return 0;
}
