#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// define "x" for system malloc, include for our versions. Don't do both.
#ifdef SYSTEM_MALLOC
#define xfree free
#define xmalloc malloc
#else
#include "malloc.h"
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define TEST_SIZE 10

int rand_between(int min, int max) { return rand() % (max - min + 1) + min; }

char* choose_test_string(char* potential_string){

  char* test_string = "Perhaps these intermittent periods of sadness "
                      "serve as the darker hues of a complex and "
                      "beautiful work of art.";

  if (potential_string != NULL) {
    test_string = potential_string;
  }

  return test_string;

}


int main(int argc, char *argv[]) {

  #ifndef SYSTEM_MALLOC
  initialize_heap();
  #else
  #endif

  srand(time(NULL));

  char *test_string = choose_test_string(argv[1]);

  char *ptrs[TEST_SIZE];

  for (int ix = 0; ix < TEST_SIZE; ix++) {
    int size = rand_between(1, strlen(test_string)+1);
    fprintf(stderr, "\n[%d] size: %d\n", ix, size);

    ptrs[ix] = (char*)xmalloc(size);

    if (ptrs[ix] == NULL) {
      printf("[%d] malloc failed\n", ix);
      exit(1);
    }

    int len_to_copy = MIN(strlen(test_string), size - 1);

    strncpy(ptrs[ix], test_string, len_to_copy);

    // sets terminator to end of the pointer 
    ptrs[ix][len_to_copy] = '\0';

    fprintf(stderr, "[%x] '%s'\n\n\n", ix, ptrs[ix]);

    if(ix == 12){
      fprintf(stderr, "\n[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
      xfree(ptrs[ix]);
      fprintf(stderr, "[%d] freed %p\n\n", ix, ptrs[ix]);
    }


    // int index_to_free = rand_between(0, ix);
    // if (ptrs[index_to_free]) {
    //   fprintf(stderr, "\n[%d] randomly freeing %p ('%s')\n", index_to_free,
    //           ptrs[index_to_free], ptrs[index_to_free]);
    //   xfree(ptrs[index_to_free]);
    //   fprintf(stderr, "[%d] freed %p\n", index_to_free, ptrs[index_to_free]);
    //   ptrs[index_to_free] = NULL;
    // }

  }

  for (int ix = 0; ix < TEST_SIZE; ix++) {
    if (ptrs[ix]) {
      fprintf(stderr, "\n[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
      xfree(ptrs[ix]);
      fprintf(stderr, "[%d] freed %p\n\n", ix, ptrs[ix]);
    } else {
      fprintf(stderr, "[%d] already freed\n", ix);
    }
  }

  return 0;
}
