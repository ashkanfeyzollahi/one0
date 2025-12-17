#include <errno.h>
#include <stdbool.h>
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

int32_t gc_list_contains(void *ptr)
{
    int32_t index = -1;
    for (size_t i = 0; i < gc_list_.count; i++)
    {
        if (gc_list_.items[i] == ptr)
        {
            index = i;
            break;
        }
    }
    return index;
}

bool gc_list_update_ptr(void *ptr_a, void *ptr_b)
{
    int32_t ptr_a_index = gc_list_contains(ptr_a);
    if (ptr_a_index == -1)
        return false;
    gc_list_.items[ptr_a_index] = ptr_b;
    return true;
}

void gc_list_swap_ptrs(int32_t ptr_a_index, int32_t ptr_b_index)
{
    void *ptr_a = gc_list_.items[ptr_a_index];
    gc_list_.items[ptr_a_index] = gc_list_.items[ptr_b_index];
    gc_list_.items[ptr_b_index] = ptr_a;
}

void gc_list_remove_ptr(int32_t ptr_index)
{
    if (ptr_index == -1)
        return;
    int32_t last_ptr_index = gc_list_.count - 1;
    if (ptr_index != last_ptr_index)
    {
        void *last_ptr = gc_list_.items[last_ptr_index];
        gc_list_.items[last_ptr_index] = gc_list_.items[ptr_index];
        gc_list_.items[ptr_index] = last_ptr;
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
