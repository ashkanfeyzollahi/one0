#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "error.h"
#include "eval.h"
#include "memory/gc.h"
#include "memory/wrappers.h"
#include "utils.h"

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
