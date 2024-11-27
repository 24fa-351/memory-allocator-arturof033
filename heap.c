
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "heap.h" 

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {
    heap_t *new_heap_t = (heap_t *)sbrk(0);
    sbrk(sizeof(heap_t));
    new_heap_t->size = 0;
    new_heap_t->capacity = capacity;
    new_heap_t->data = sbrk(0);
    sbrk(capacity * sizeof(heap_node_t));
    
    for (int ix = 0; ix < new_heap_t->capacity; ix++)
    {
        new_heap_t->data[ix].key = -1;
        new_heap_t->data[ix].value = NULL;
    }
    
    return new_heap_t;
}

void heap_free(heap_t *heap) {

    for(int ix = 0; ix < heap->size; ix++)
    {
        free(heap->data[ix].value);
    }

    free(heap->data);
    free(heap);
}

unsigned int heap_size(heap_t *heap) { return heap->size; }

unsigned int heap_parent(unsigned int index) { return  (index - 1) / 2; }

unsigned int heap_left_child(unsigned int index) { return (index * 2); }

unsigned int heap_right_child(unsigned int index) { return (index * 2 + 1); }

unsigned int heap_level(unsigned int index) { 

    if(index == 0)
    {
        return 0;
    }

    index = (index % 2 ? index + 1: index);

    int level = 0;
    while (index >= 2)
    {
        index = index / 2;
        level++;
    }


    return level;

}

void heap_print(heap_t *heap) {

    for (unsigned int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {

    heap_node_t tempNode = heap->data[index1];
    heap->data[index1] = heap->data[index2];
    heap->data[index2] = tempNode;

}

void heap_bubble_up(heap_t *heap, int index) {

    while (index > 0 && heap->data[heap_parent(index)].key > heap->data[index].key )
    {
        heap_swap(heap, index, heap_parent(index));
        index = heap_parent(index);
    }

}

void heap_bubble_down(heap_t *heap, int index) {

    if(heap_size(heap)== 1)
    {
        return;
    }

    unsigned int left_index, right_index, smaller_index;
    heap_key_t left_key, right_key, current_key;

    while(index < (int)heap_size(heap))
    {

        left_index = heap_left_child(index);
        right_index = heap_right_child(index);
        smaller_index = index;

        left_key = heap->data[left_index].key;
        right_key = heap->data[right_index].key;
        current_key = heap->data[smaller_index].key;

        if(left_index > heap_size(heap) && left_key < current_key){
            smaller_index = left_index;
        }
        if(right_index > heap_size(heap) && right_key < current_key){
            smaller_index = left_index;
        }
        if(index == (int)smaller_index){
            break;
        }
        
        heap_swap(heap, index, smaller_index);
        index = smaller_index;

    }

}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == (unsigned int)heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return NULL;
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}
