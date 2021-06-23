#include "binaryheap.h"
#include <assert.h>
#include <string.h>

#define NULLCHECK(ptr, msg) if (!ptr) {printf(msg); exit(1);}


/*allocate memory in the heap for a node_heap type element and returns a pointer to the new node_heap*/

node_heap *allocate_node_heap(void)
{
  node_heap* node = calloc(1, sizeof(node_heap));
  NULLCHECK(node, "heap node could not be allocated")
  node->key = malloc(1);
  NULLCHECK(node->key, "heap node value allocation failure")
  return node;
}

/*initialise the heap array elements*/


/*heap already set up with correct length, starts with a NULL*/
void initial_heap(node_heap **heap,char* sequence)
{
    /* iterate until at end of string, for character at pos x, place into pos x + 1 in heap */
    for (int i = 1; sequence[i-1]; i++) {
        heap[i] = allocate_node_heap();
        *(heap[i]->key) = sequence[i-1];
        heap[i]->position = i;
    }
}


/*print every element of the heap array*/
void print_elem_heap(node_heap **heap, int length)
{
    for (int i = 1; i <= length; i++) {
        if (heap[i]) printf("%s ", heap[i]->key);
    }
    printf("\n");
    for (int i = 1; i <= length; i++) printf("%i ", heap[i]->position);
    printf("\n");
}

/* returns the index in the heap array where the parent is allocated for the index passed as argument*/

int parent(int index)
{
    return index >> 1;
}

/* returns the index in the heap array where the left child is allocated for the index passed as argument*/

int left_child(int index)
{
    return index << 1;
}

/* returns the index in the heap array where the right child is allocated for the index passed as argument*/

int right_child(int index)
{
    return (index << 1) + 1;
}

/* exchange node_heap node1 to node_heap node2*/ 

void swap(node_heap *node1, node_heap *node2)
{
    char* keyholder = node1->key;
    int positionholder = node1->position;

    /* copy node 2 into node 1 */
    node1->key = node2->key;
    node1->position = node2->position;

    /* copy held node 1 values into node 2 */
    node2->key = keyholder;
    node2->position = positionholder;
}

/*Moves down the value of the heap[current] so the subtree rooted at index "current" satisfies with the max-heap property*/ 

void max_heapify(node_heap **heap, int current, int heap_size)
{   
    int largest_subnode = 0;

    if (right_child(current) <= heap_size) {
        int left = left_child(current);
        int right = right_child(current);

        if (heap[left]->key[0] > heap[right]->key[0] && heap[left]->key[0] > heap[current]->key[0]) {
            largest_subnode = left;
        } else if (heap[right]->key[0] > heap[current]->key[0]) {
            largest_subnode = right;
        }

    } else if (left_child(current) <= heap_size) {
         int left = left_child(current);
        if (heap[left]->key[0] > heap[current]->key[0]) {
            largest_subnode = left;
        }
    }

    if (largest_subnode) {
        swap(heap[largest_subnode], heap[current]);
        max_heapify(heap, largest_subnode, heap_size);
    }
}

/*it orders the heap so the ordered heap complies the max-heap property*/

void build_max_heap(node_heap **heap, int heap_size)
{
    for(int i = heap_size >> 1; i > 0; i--) {
        max_heapify(heap, i, heap_size);
    }
}

/*starting from a max-heap ordered array, it moves the largest item from the heap and it into the array position left as the heap shrinks*/

void heapsort(node_heap **heap, int length)
{
   build_max_heap(heap, length);
   for (int i = length; i > 0;) {
       swap(heap[1], heap[i]);
       i--;
       max_heapify(heap, 1, i);
   }
    
}

/*free the memory allocated by a node_heap type element in the heap*/

void free_node(node_heap *node)
{
    free(node->key);
    free(node);
}

/*free the memory allocated in the heap array*/

void free_heap(node_heap **heap, int length)
{
    for (int i = 1; i <= length; i++) free_node(heap[i]);
    free(heap);
}