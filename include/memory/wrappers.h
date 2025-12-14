#ifndef MEMORY_WRAPPERS_H
#define MEMORY_WRAPPERS_H

#include <stddef.h>

void xfree(void *__ptr);
void *xmalloc(size_t __size);
void *xrealloc(void *__ptr, size_t __size);

#endif
