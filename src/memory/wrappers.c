#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "memory/gc.h"

void xfree(void *__ptr)
{
    int32_t ptr_index = gc_list_contains(__ptr);
    gc_list_remove_ptr(ptr_index);
    if (ptr_index == -1)
        panic("Freeing memory that wasn't being tracked by gc.");
    free(__ptr);
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

void *xrealloc(void *__ptr, size_t __size)
{
    void *ptr = xngcrealloc(__ptr, __size);
    bool ptr_updated = gc_list_update_ptr(__ptr, ptr);
    if (!ptr_updated)
        gc_list_add_ptr(ptr);
    return ptr;
}

char *xstrdup(const char *__s)
{
    char *s = strdup(__s);
    if (s == NULL)
    {
        errno = ENOMEM;
        panic(NULL);
    }
    gc_list_add_ptr(s);
    return s;
}
