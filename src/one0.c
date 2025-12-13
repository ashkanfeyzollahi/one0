#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/config.h"
#include "../include/errors.h"
#include "../include/functions.h"

char *progmem;

static error_t
parse_opt(int __key, char *__arg, struct argp_state *__state)
{
    char **filename = (char **)__state->input;
    switch (__key)
    {
    case 'i':
        *filename = __arg;
        break;
    }
    return 0;
}

typedef struct gc_list
{
    void **items;
    size_t count;
} gc_list;

gc_list gc_list_ = {NULL, 0};

void xfree(void *__ptr);
void *xmalloc(size_t __size);
void *xngcrealloc(void *__ptr, size_t __size);
void *xrealloc(void *__ptr, size_t __size);

void gc_list_add_ptr(void *ptr)
{
    gc_list_.items = xngcrealloc(gc_list_.items, sizeof(void *) * (gc_list_.count + 1));
    gc_list_.items[gc_list_.count++] = ptr;
}

void gc_list_remove_ptr(void *ptr)
{
    size_t index = -1;
    for (int i = 0; i < gc_list_.count; i++)
    {
        if (gc_list_.items[i] == ptr)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    void *last_ptr = gc_list_.items[gc_list_.count - 1];
    if (last_ptr != ptr)
    {
        gc_list_.items[gc_list_.count - 1] = ptr;
        gc_list_.items[index] = last_ptr;
    }
    gc_list_.items = xngcrealloc(gc_list_.items, sizeof(void *) * --gc_list_.count);
}

void collect_garbages()
{
    for (int i = 0; i < gc_list_.count; i++)
    {
        free(gc_list_.items[i]);
    }
    gc_list_.count = 0;
    free(gc_list_.items);
}

void panic(char *msg)
{
    if (msg == NULL)
        perror("Oops");
    else
        fprintf(stderr, "Oops: %s\n", msg);
    collect_garbages();
    exit(errno && errno <= 255 ? errno : 1);
}

void xfree(void *__ptr)
{
    for (int i = 0; i < gc_list_.count; i++)
    {
        if (gc_list_.items[i] == __ptr)
        {
            gc_list_remove_ptr(__ptr);
            free(__ptr);
            break;
        }
    }
}

void *xmalloc(size_t __size)
{
    void *ptr = malloc(__size);
    if (ptr == NULL)
    {
        errno = ENOMEM;
        panic(NULL);
    }
    gc_list_add_ptr(ptr);
    return ptr;
}

void *xngcrealloc(void *__ptr, size_t __size)
{
    void *tmp = realloc(__ptr, __size);
    if (tmp == NULL)
    {
        errno = ENOMEM;
        panic(NULL);
    }
    return tmp;
}

void *xrealloc(void *__ptr, size_t __size)
{
    void *ptr = xngcrealloc(__ptr, __size);
    for (int i = 0; i < gc_list_.count; i++)
    {
        if (gc_list_.items[i] == __ptr)
            gc_list_.items[i] = ptr;
    }
    return ptr;
}

char **strtoks(const char *__s, const char *__delim)
{
    char *copy = strdup(__s);

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

size_t get_func_count()
{
    size_t count = 0;
    while (func_ptrs[count])
        count++;
    return count;
}

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

int main(int argc, char **argv)
{
    const struct argp_option options[] = {
        {"input", 'i', "FILENAME", 0, "Path to the source file to interpret."},
        {0}};
    const struct argp argp_ = {options, parse_opt};

    char **filename = (char **)xmalloc(sizeof(char *));

    argp_parse(&argp_, argc, argv, 0, 0, filename);

    if (*filename == NULL)
        *filename = "main.o0";

    FILE *f = fopen(*filename, "r");

    if (f == NULL)
        panic(NULL);

    char **proglines = fgetlines(f);

    progmem = (char *)xmalloc(PROGMEMLIMIT);
    memset(progmem, 0, PROGMEMLIMIT);
    eval_prog(proglines);

    fclose(f);

    collect_garbages();
    return EXIT_SUCCESS;
}
