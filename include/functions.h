#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./config.h"
#include "./errors.h"

size_t goto_ = 0;
size_t progmem_curpos = 0;

void print(char *args, char *progmem)
{
    printf("%s", args);
}

void printmem(char *args, char *progmem)
{
    printf("%s", progmem);
}

void printmembyte(char *args, char *progmem)
{
    printf("%c", progmem[progmem_curpos]);
}

void moveptrr(char *args, char *progmem)
{
    progmem_curpos++;
}

void moveptrl(char *args, char *progmem)
{
    progmem_curpos--;
}

void incrementptrbyte(char *args, char *progmem)
{
    progmem[progmem_curpos]++;
}

void decrementptrbyte(char *args, char *progmem)
{
    progmem[progmem_curpos]--;
}

void inputch(char *args, char *progmem)
{
    progmem[progmem_curpos] = (char)getc(stdin);
}

void inputs(char *args, char *progmem)
{
    char buffer[1024];
    fgets(buffer, 1024, stdin);

    if (PROGMEMLIMIT - progmem_curpos < 1024)
    {
        cerrno = 1;
        return;
    }

    memcpy(&progmem[progmem_curpos], buffer, 1024);
}

void gotoline(char *args, char *progmem)
{
    int line = 0;
    size_t args_size = strlen(args);
    for (int i = 0; i < args_size; i++)
        line = line | (args[i] << ((args_size - i - 1) * 8));
    goto_ = line;
}

void gotolineif(char *args, char *progmem)
{
    size_t args_size = strlen(args);
    if (args_size < 2)
    {
        cerrno = 2;
        return;
    }
    if (progmem[progmem_curpos] == args[0])
        gotoline(&args[1], progmem);
}

void gotolineifn(char *args, char *progmem)
{
    size_t args_size = strlen(args);
    if (args_size < 2)
    {
        cerrno = 2;
        return;
    }
    if (progmem[progmem_curpos] != args[0])
        gotoline(&args[1], progmem);
}

void (*func_ptrs[])(char *args, char *progmem) = {
    print,
    printmem,
    printmembyte,
    moveptrr,
    moveptrl,
    incrementptrbyte,
    decrementptrbyte,
    inputch,
    inputs,
    gotoline,
    gotolineif,
    gotolineifn,
    NULL};

#endif
