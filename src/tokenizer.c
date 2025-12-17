#include <stdlib.h>
#include <string.h>

#include "memory/wrappers.h"

char **strtoks(const char *__s, const char *__delim)
{
    char *copy = xstrdup(__s);

    size_t count = 0;
    char **out = NULL;

    char *tok = strtok(copy, __delim);

    if (tok == NULL)
        return NULL;

    out = (char **)xmalloc(sizeof(char *));
    out[count++] = tok;

    while ((tok = strtok(NULL, __delim)) != NULL)
    {
        out = (char **)xrealloc(out, sizeof(char *) * (count + 1));
        out[count++] = tok;
    }

    out = (char **)xrealloc(out, sizeof(char *) * (count + 1));
    out[count] = NULL;

    return out;
}
