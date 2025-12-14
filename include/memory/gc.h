#ifndef MEMORY_GC_H
#define MEMORY_GC_H

#include <stddef.h>

void *xngcrealloc(void *__ptr, size_t __size);

typedef struct gc_list
{
    void **items;
    size_t count;
} gc_list;

extern gc_list gc_list_;
void gc_list_add_ptr(void *ptr);
void gc_list_remove_ptr(void *ptr);
void collect_garbages();

#endif
