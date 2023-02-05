#ifndef PROMPT_LINE_H
#define PROMPT_LINE_H

// for FILE
#include <stdio.h>

#define PROMPT_LINE_MIN_SIZE (32)
#define PROMPT_LINE_MAX_SIZE (1024*1024)

size_t prompt_line(FILE *in, FILE *out, const char* prompt, char **const ret);

#endif // PROMPT_LINE_H
