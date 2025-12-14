#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>

extern size_t goto_;
extern size_t progmem_curpos;
void print(char *args, char *progmem);
void printmem(char *args, char *progmem);
void printmembyte(char *args, char *progmem);
void moveptrr(char *args, char *progmem);
void moveptrl(char *args, char *progmem);
void incrementptrbyte(char *args, char *progmem);
void decrementptrbyte(char *args, char *progmem);
void inputch(char *args, char *progmem);
void inputs(char *args, char *progmem);
void gotoline(char *args, char *progmem);
void gotolineif(char *args, char *progmem);
void gotolineifn(char *args, char *progmem);
extern void (*func_ptrs[])(char *args, char *progmem);
size_t get_func_count();

#endif
