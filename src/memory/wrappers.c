#include <errno.h>
#include <stdlib.h>

#include "error.h"
#include "memory/gc.h"

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
