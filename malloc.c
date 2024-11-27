#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "malloc.h"

//memory_block *memory[HEAP_SIZE];

int rand_between(int min, int max) { return rand() % (max - min + 1) + min; }

heap_t *memory;

void initialize_heap(){

    srand(time(NULL));

    memory = heap_create(HEAP_SIZE);

    fill_memory();

}


memory_block* allocate_memory_block(int size_of_memory_block){

    memory_block* new_memory_block = sbrk(0);

    sbrk(sizeof(memory_block));

    new_memory_block->ptr_to_start = sbrk(0);
    new_memory_block->size = size_of_memory_block;
    new_memory_block->is_empty = true;

    sbrk(size_of_memory_block);

    return new_memory_block;

}


void free_heap(){

    for(int ix = 0; ix < HEAP_SIZE; ix++){

        memory_block *block_to_delete = get_memory_block_from_ptr(memory->data[ix].value);

        free_memory_block(&block_to_delete);

    }

}


void free_memory_block(memory_block** memory_to_delete){

    free(*memory_to_delete);

}


void* xmalloc(size_t size) {

    char *ptr = get_memory_block_of_size(size);

    if(ptr == NULL){
        fprintf(stderr, __FILE__ ": %d malloc failed\n", __LINE__);
        exit(1);
    }

    fprintf(stderr, __FILE__ ":%d malloc(%lu) = %p\n", __LINE__, size, ptr);

    return ptr;
}


void* get_memory_block_of_size(size_t requested_size){
    
    for(int ix = 0; ix < HEAP_SIZE; ix++){

        memory_block *current_memory_block = get_memory_block_from_ptr(memory->data[ix].value);

        if(current_memory_block->is_empty){

            if(current_memory_block->size >= requested_size){

                current_memory_block->is_empty = false;
                return current_memory_block->ptr_to_start;

            }

            // else if (ix + 1 < HEAP_SIZE && get_memory_block_from_ptr(memory->data[ix + 1].value)->is_empty) {
                
            //     int iy = ix + 1;
            //     int total_size = get_memory_block_from_ptr(memory->data[ix].value)->size;

            //     while(iy < HEAP_SIZE && 
            //           get_memory_block_from_ptr(memory->data[iy].value)->is_empty && 
            //           total_size < requested_size){

            //         memory_block *potential_memory_to_merge = get_memory_block_from_ptr(memory->data[iy].value);

            //         total_size += potential_memory_to_merge->size + sizeof(*potential_memory_to_merge);


            //         iy++;

            //     }

            //     if(total_size >= requested_size){

            //         heap_print(memory);

            //         for(int curr_index = ix + 1; curr_index < iy; curr_index++){

            //             memory_block* memory_block_to_merge = get_memory_block_from_ptr(memory->data[curr_index].value);
            //             merge_with(current_memory_block, memory_block_to_merge);

            //         }

            //         current_memory_block->is_empty = false;
            //         return current_memory_block->ptr_to_start;
                    
            //     }
            //     else{

            //         return NULL;

            //     }

            // }
            
        }


    }

    return NULL;

}

void merge_with(memory_block* current_memory_block, memory_block* memory_to_merge){

    current_memory_block->size += memory_to_merge->size + sizeof(*memory_to_merge);
    
    fprintf(stderr, __FILE__ ": we just added %d and %lu\n", memory_to_merge->size, sizeof(*memory_to_merge));
    fprintf(stderr, __FILE__ ": after merge we have size %d\n", current_memory_block->size);

    memory_to_merge->size = 0;
    memory_to_merge->is_empty = true;
    memory_to_merge->ptr_to_start = NULL;



    memory->size -= 1;

}

void fill_memory(){

    while(memory->size < memory->capacity){

        // int memory_block_size = rand_between(5, 20);

        int memory_block_size = DEFAULT_MEMORY_BLOCK_SIZE;

        fprintf(stderr, __FILE__ ": allocating data of size %d", memory_block_size);

        memory_block *new_memory_block = allocate_memory_block(memory_block_size);

        heap_insert(memory, new_memory_block->size, new_memory_block->ptr_to_start);

    }

}


void xfree(void *ptr) {

    fprintf(stderr, __FILE__ ": free(%p)\n", ptr);

    return_to_memory(ptr);

}


void return_to_memory(void *ptr){

    memory_block* current_memory_block = get_memory_block_from_ptr(ptr);
    current_memory_block->is_empty = true;

}

memory_block* get_memory_block_from_ptr(void *ptr){

    return (memory_block*)((void*)ptr - sizeof(memory_block));;

}
