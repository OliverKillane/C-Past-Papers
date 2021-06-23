#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exam.h"

#define NULLCHECK(ptr, msg) if (!ptr) {perror(msg);exit(EXIT_FAILURE);}

/*
 * Private function prototypes.
 */

static void _print_huffman_tree(const huffman_tree_t *, int);
static void _print_huffman_tree_codes(const huffman_tree_t *, char *, char *);

/*
 * Prints the given Huffman tree.
 */
void print_huffman_tree(const huffman_tree_t *t) {
  printf("Huffman tree:\n");
  _print_huffman_tree(t, 0);
}

/*
 * Private helper function for print_huffman_tree.
 */
static void _print_huffman_tree(const huffman_tree_t *t, int level) {
  int i;
  for (i = 0; i <= level; i++) {
    printf("  ");
  }

  if (t->left == NULL && t->right == NULL) {
    printf("Leaf: '%c' with count %d\n", t->letter, t->count);
  } else {
    printf("Node: accumulated count %d\n", t->count);

    if (t->left != NULL) {
      _print_huffman_tree(t->left, level + 1);
    }

    if (t->right != NULL) {
      _print_huffman_tree(t->right, level + 1);
    }
  }
}

/*
 * Prints the codes contained in the given Huffman tree.
 */
void print_huffman_tree_codes(const huffman_tree_t *t) {
  printf("Huffman tree codes:\n");

  char *code = calloc(MAX_CODE_LENGTH, sizeof(char)), *code_position = code;
  if (code == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  _print_huffman_tree_codes(t, code, code_position);
  free(code);
}

/*
 * Private helper function for print_huffman_tree_codes.
 */
static void _print_huffman_tree_codes(const huffman_tree_t *t,
                                      char *code, char *code_position) {

  if (t->left == NULL && t->right == NULL) {
    printf("'%c' has code \"%s\"\n", t->letter, code);
  } else if (t->left != NULL) {
    *code_position++ = 'L';
    *code_position = '\0';

    _print_huffman_tree_codes(t->left, code, code_position--);
  }

  if (t->right != NULL) {
    *code_position++ = 'R';
    *code_position = '\0';

    _print_huffman_tree_codes(t->right, code, code_position--);
  }
}

/*
 * Prints a list of Huffman trees.
 */
void print_huffman_tree_list(const huffman_tree_list_t *l) {
  printf("Huffman tree list:\n");

  for (; l != NULL; l = l->next) {
    print_huffman_tree(l->tree);
  }
}

/*
 * Frees a Huffman tree.
 */
void huffman_tree_free(huffman_tree_t *t) {
  if (t != NULL) {
    huffman_tree_free(t->left);
    huffman_tree_free(t->right);
    free(t);
  }
}

/*
 * Frees a list of Huffman trees.
 */
void huffman_tree_list_free(huffman_tree_list_t *l) {
  if (l != NULL) {
    huffman_tree_list_free(l->next);
    huffman_tree_free(l->tree);
    free(l);
  }
}

/*
 * Returns 1 if the string s contains the character c and 0 if it does not.
 */
int contains(char *s, char c) {
  for (;*s;s++) if (*s == c) return 1;
  return 0;
}

/*
 * Returns the number of occurrences of c in s.
 */
int frequency(char *s, char c) {
  int freq = 0;
  for (;*s;s++) if (*s == c) freq++;
  return freq;
}

/*
 * Takes a string s and returns a new heap-allocated string containing only the
 * unique characters of s.
 *
 * Pre: all strings will have fewer than or equal to MAX_STRING_LENGTH - 1
 *      characters.
 */
char *nub(char *s) {
  char *str = calloc(MAX_STRING_LENGTH, 1);
  NULLCHECK(str, "nub: str allocation failed")
  char *end = str;
  for (;*s;s++) {
    if(!contains(str, *s)) {
      *end = *s;
      end++;
    }
  }
  str = realloc(str, end - str + 1);
  NULLCHECK(str, "nub: str reallocation failed")
  return str;
}

/*
 * Adds the Huffman tree t to the list l, returning a pointer to the new list.
 *
 * Pre:   The list l is sorted according to the frequency counts of the trees
 *        it contains.
 *
 * Post:  The list l is sorted according to the frequency counts of the trees
 *        it contains.
 */
huffman_tree_list_t *huffman_tree_list_add(huffman_tree_list_t *l, huffman_tree_t *t) {
  
  /* sanity check */
  assert(l && t);

  /* allocate memory with null check*/
  huffman_tree_list_t *new_elem = malloc(sizeof(huffman_tree_list_t));
  NULLCHECK(new_elem, "huffman_tree_list_add: new_elem allocation failed")

  /* store tree pointer */
  new_elem->tree = t;

  if (t->count > l->tree->count) {

    /* prepend new element */
    new_elem->next = l;
    return new_elem;

  } else {

    /* iterate through the list */
    huffman_tree_list_t *lst = l;

    /* move until t has count larger than the next after lst, or at list end*/
    for (; lst->next && t->count > lst->next->tree->count; lst = lst->next);
  
    /* if the next one is larger, we stop to insert*/
    /* if not smaller than any item, reaches end, we append (lst will be at the end)*/
    
    /* link: lst, new_elem, lst->next*/
    new_elem->next = lst->next;
    lst->next = new_elem;

    /* return the list pointer*/
    return l;
  }
}


/* new tree function */
huffman_tree_t *new_tree(int count, char letter) {

  /* allocate tree memory */
  huffman_tree_t *newtree = malloc(sizeof(huffman_tree_t));
  NULLCHECK(newtree, "huffman_tree_list_build: new tree allocation failed")

  /* set members */
  newtree->count = count;
  newtree->letter = letter;

  return newtree;
}

/*
 * Takes a string s and a lookup table and builds a list of Huffman trees
 * containing leaf nodes for the characters contained in the lookup table. The
 * leaf nodes' frequency counts are derived from the string s.
 *
 * Pre:   t is a duplicate-free version of s.
 *
 * Post:  The resulting list is sorted according to the frequency counts of the
 *        trees it contains.
 */
huffman_tree_list_t *huffman_tree_list_build(char *s, char *t) {

  /* sanity check */
  assert(s && t);

  /* allocate a new tree, with first element */
  huffman_tree_list_t *list = malloc(sizeof(huffman_tree_list_t));
  NULLCHECK(list, "huffman_tree_list_build: list allocation failed")

  /* insert first element */
  list->tree = new_tree(frequency(s, *t), *t);
  list->next = NULL;

  /* move to next character */
  t++;

  /*create huffman tree and insert */
  for (; *t; t++) huffman_tree_list_add(list, new_tree(frequency(s, *t), *t));
}

/*
 * Reduces a sorted list of Huffman trees to a single element.
 *
 * Pre:   The list l is non-empty and sorted according to the frequency counts
 *        of the trees it contains.
 *
 * Post:  The resuling list contains a single, correctly-formed Huffman tree.
 */
huffman_tree_list_t *huffman_tree_list_reduce(huffman_tree_list_t *l) {
  huffman_tree_t *left;
  huffman_tree_t *right;
  huffman_tree_list_t* next_l;

  while(l->next) {
    /* combine top two trees and add together (larger on the right) */
    
    /*get left and right subtrees */
    left = l->next->tree;
    right = l->tree;

    /* get the new list beginning */
    next_l = l->next->next;

    /* free the trees*/
    free(l->next);
    free(l);

    /* create new tree */
    huffman_tree_t *new_tree = malloc(sizeof(huffman_tree_t));
    new_tree->left = left;
    new_tree->right = right;
    new_tree->count = left->count + right->count;

    /* insert into the list */
    l = huffman_tree_list_add(next_l, new_tree);
  }

  return l;
}



// BECAME TO TRIRED WILL CONTINUE
void build_encoding_table(char *passed, huffman_tree_t *t) {

}

/*
 * Accepts a Huffman tree t and a string s and returns a new heap-allocated
 * string containing the encoding of s as per the tree t.
 *
 * Pre: s only contains characters present in the tree t.
 */
char *huffman_tree_encode(huffman_tree_t *t, char *s) {
  char* mapping[128] = calloc(128, sizeof(char*));



}

/*
 * Accepts a Huffman tree t and an encoded string and returns a new
 * heap-allocated string contained the decoding of the code as per the tree t.
 *
 * Pre: the code given is decodable using the supplied tree t.
 */
char *huffman_tree_decode(huffman_tree_t *t, char *code) {
  return NULL;
}
