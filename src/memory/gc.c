#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "memory/gc.h"

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

gc_list gc_list_ = {NULL, 0};

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
    for (size_t i = 0; i < gc_list_.count; i++)
        free(gc_list_.items[i]);
    gc_list_.count = 0;
    free(gc_list_.items);
}
