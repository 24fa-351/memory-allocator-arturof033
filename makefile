all: malloc_test_sys malloc_test_dev

malloc_test_sys: malloc_test.c
	gcc -o malloc_test_sys -DSYSTEM_MALLOC malloc_test.c

malloc_test_dev: malloc_test.c malloc.c malloc.h heap.c heap.h
	gcc -o malloc_test_dev malloc_test.c malloc.c heap.c

clean:
	rm -f malloc_test_sys malloc_test_dev
