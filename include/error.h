#ifndef ERRORS_H
#define ERRORS_H

extern char cerrno;
extern char *cerrmsgs[];
char *cstrerror();
void panic(char *msg);

#endif
