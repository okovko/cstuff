// for prompt_line, PROMPT_LINE_MIN_SIZE, PROMPT_LINE_MAX_SIZE
#include "prompt_line.h"

// for ptrdiff_t, size_t
#include <stddef.h>
// for fprintf, stdin, stdout, FILE, ferror, feof, fgets
#include <stdio.h>
// for realloc, free
#include <stdlib.h>
// for memchr, memset
#include <string.h>
// for true
#include <stdbool.h>

// similar to posix getline
size_t prompt_line(FILE *in, FILE *out, const char* prompt, char **const ret) {
  // the loop logic depends on these specific init values
  size_t size = PROMPT_LINE_MIN_SIZE;
  size_t len = 0;
  char *line = NULL;

  // print the prompt, if possible
  if (out && !ferror(out) && prompt) {
    fprintf(out, "%s", prompt);
  }

  while (true) {
    // preallocate buffer to read into
    // when passed NULL, realloc is identical to malloc
    // realloc(..., 0) is undefined, (size + 1) is important
    line = realloc(line, size + 1);
    if (!line) break;

    // read up through (size - len), leaving a byte for '\0'
    // fgets(...) reads one less than its size argument, (read_max + 1) is important
    // ferror(...) does not distinguish between error and eof, need to check feof(...) too
    // fgets(...) is used because:
    // read(...) is posix non-standard, read(...) and fread(...) do not terminate until eof (ctrl-D)
    // (so the program would hang until eof, no matter how many newlines are entered)
    // despite fgets(...) having confusing and error prone semantics, the only other (worse) choice is fgetc(...)
    size_t read_max = size - len;
    char *from = line + len;
    from = fgets(from, read_max + 1, in);
    if (!from || (ferror(in) && !feof(in))) break;

    // search for '\n' up through read_max
    // last is null, or last > from
    // read_count <= read_max
    char *const last = memchr(from, '\n', read_max);
    size_t read_count = last ? (size_t)(last - from) : read_max;
    len += read_count;

    // if done reading, zero the remaining bytes (at least one)
    // otherwise, double the allocation for the next iteration, up to the limit
    if (last) {
      // this return path is for the success case
      memset(last, '\0', read_max - read_count + 1);
      *ret = line;
      return len;
    } else {
      size = (size < PROMPT_LINE_MAX_SIZE ? size * 2 : PROMPT_LINE_MAX_SIZE);
    }
  }

  // deallocate if breaking out of loop due to error
  if (line) {
    free(line);
  }

  // this return path is for the error case
  *ret = NULL;
  return 0;
}
