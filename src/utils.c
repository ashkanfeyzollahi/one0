#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "memory/wrappers.h"

char *fgetline(FILE *__stream)
{
    char *line = NULL;
    char buffer[1024];
    size_t line_size = 0;

    while (fgets(buffer, sizeof buffer, __stream) != NULL)
    {
        size_t buffer_size = strlen(buffer);

        size_t new_size = line_size + buffer_size;

        line = (char *)xrealloc(line, new_size + 1);
        memcpy(line + line_size, buffer, buffer_size);
        line_size = new_size;
        line[line_size] = '\0';

        if (line_size > 0 && line[line_size - 1] == '\n')
        {
            line[--line_size] = '\0';
            break;
        }
    }

    if (ferror(__stream))
        panic(NULL);

    return line;
}

char **fgetlines(FILE *__stream)
{
    char **lines = NULL;
    size_t lines_count = 0;
    char *line = fgetline(__stream);

    while (line != NULL)
    {
        lines = (char **)xrealloc(lines, sizeof(char *) * (lines_count + 1));
        lines[lines_count++] = line;
        line = fgetline(__stream);
    }

    lines = (char **)xrealloc(lines, sizeof(char *) * (lines_count + 1));
    lines[lines_count++] = NULL;

    return lines;
}

char bin_to_ch(const char *s)
{
    char ch;
    size_t slen = strlen(s);

    if (slen > 8)
        panic("Binary numbers must not be longer than 8 characters");

    for (int i = 0; i < slen; i++)
        ch = ch | ((s[i] == '1' ? 1 : 0) << (slen - i - 1));

    return ch;
}
