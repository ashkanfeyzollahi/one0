#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

char *fgetline(FILE *__stream);
char **fgetlines(FILE *__stream);
char bin_to_ch(const char *s);

#endif
