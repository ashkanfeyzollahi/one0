#ifndef MEMORY_GC_H
#define MEMORY_GC_H

#include <stdbool.h>
#include <stddef.h>

void *xngcrealloc(void *__ptr, size_t __size);

typedef struct gc_list
{
    void **items;
    size_t count;
} gc_list;

extern gc_list gc_list_;
void gc_list_add_ptr(void *ptr);
int32_t gc_list_contains(void *ptr);
bool gc_list_update_ptr(void *ptr_a, void *ptr_b);
void gc_list_swap_ptrs(int32_t ptr_a_index, int32_t ptr_b_index);
void gc_list_remove_ptr(int32_t ptr_index);
void collect_garbages();

#endif
