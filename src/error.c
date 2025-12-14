#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory/gc.h"

char cerrno = 0;

char *cerrmsgs[] = {
    "No error"
    "Memory limit exceeded"
    "Not enough arguments"};

char *cstrerror()
{
    return cerrmsgs[(int)cerrno];
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
