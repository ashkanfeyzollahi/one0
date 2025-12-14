#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "functions.h"
#include "memory/wrappers.h"
#include "tokenizer.h"
#include "utils.h"

char *progmem;

void eval_line(const char *line)
{
    char **toks = strtoks(line, " ");

    if (toks[0] == NULL)
        return;

    char func = bin_to_ch(toks[0]);

    if (func >= get_func_count())
        panic("Function index exceeds function table size");

    char *args = NULL;
    size_t args_size = 0;

    for (int i = 1; toks[i] != NULL; i++)
    {
        args = (char *)xrealloc(args, args_size + 1);
        args[args_size++] = bin_to_ch(toks[i]);
    }

    args = (char *)xrealloc(args, args_size + 1);
    args[args_size++] = '\0';

    func_ptrs[(int)func](args, progmem);

    if (cerrno > 0)
        panic(cstrerror());
}

void eval_prog(char **lines)
{
    size_t lines_count = 0;

    while (lines[lines_count++] != NULL)
    {
    }

    for (int i = 0; lines[i] != NULL; i++)
    {
        if (strcmp(lines[i], "") == 0)
            continue;

        eval_line(lines[i]);

        if (goto_ > lines_count)
            panic("Line doesn't exist");

        if (goto_ > 0)
        {
            i = goto_ - 2;
            goto_ = 0;
        }
    }
}
