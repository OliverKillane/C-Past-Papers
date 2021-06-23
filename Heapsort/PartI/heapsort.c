#include "binaryheap.h"
#include <string.h>


int main(int argc, char **argv){

    if (argc == 2) {
        node_heap *heap[21];
        heap[0] = NULL;

        int length = strlen(argv[1]);

        initial_heap(heap, argv[1]);
        print_elem_heap(heap, length);
        build_max_heap(heap, length);
        print_elem_heap(heap, length);
        heapsort(heap, length);
        print_elem_heap(heap, length);
    } else {
        printf("Invalid arguments, needs 1 string");
        exit(1);
    }
    
    
    return EXIT_SUCCESS;
}