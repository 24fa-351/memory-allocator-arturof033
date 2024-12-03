#include <stdlib.h>
#include <stdbool.h>

#include "heap.h"

#ifndef MALLOC_H
#define MALLOC_H

#define HEAP_SIZE 15
#define DEFAULT_MEMORY_BLOCK_SIZE 1024

int rand_between(int min, int max);

typedef struct {
    int size;
    void *ptr_to_start;
    bool is_empty;
} memory_block;

//extern memory_block *memory[HEAP_SIZE];

extern heap_t *memory;

void initialize_heap();

memory_block* allocate_memory_block(int size_of_memory_block);

void free_heap();

void free_memory_block(memory_block** memory_to_delete);

void* xmalloc(size_t size);

void* get_memory_block_of_size(size_t size);

void merge_with(memory_block* current_memory_block, memory_block* memory_to_merge);

void fill_memory();

void xfree(void *ptr);

void return_to_memory(void *ptr);

memory_block* get_memory_block_from_ptr(void *ptr);


#endif
