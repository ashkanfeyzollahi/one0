#ifndef ERRORS_H
#define ERRORS_H

char cerrno = 0;

char *cerrmsgs[] = {
    "No error"
    "Memory limit exceeded"
    "Not enough arguments"};

char *cstrerror()
{
    return cerrmsgs[(int)cerrno];
}

#endif
