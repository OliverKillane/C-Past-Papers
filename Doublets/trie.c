#include "trie.h"
#include <assert.h>

#define ALPHABET_SIZE 26

dictionary_t *create_dict(void) {
  return create_node();
}

void free_dict(dictionary_t *root) {
  free_node(root);
}

/* ------------ YOUR CODE BELOW -------------- */

bool find(dictionary_t *root, const char *word) {
  dictionary_t* node = root;
  int current_letter;
  
  /* go through each character, if the next one is null, then check if end of word*/

  /* sanity check */
  if (!root) {
    return false;
  }

  /* iterate through every letter until the last*/
  for (int i = 0; word[i]; i++) {
    
    if (word[i] < 'A' || word[i] > 'Z') {
      return false;
    }

    current_letter = word[i] - 'A';

   /* traverse to child */
    node = node->children[current_letter];

    /* if child nonexistent, return false */
    if (!node) {
      return false;
    }
  }

  return node->end_of_word;
}

bool insert(dictionary_t *root, const char *word) {
  
  dictionary_t* node = root;
  int current_letter;

  /* for every non null character (iterating though) */
  for (int i = 0; word[i]; i++) {

    /* get the index*/
    if (word[i] < 'A' || word[i] > 'Z') {
      return false;
    }

    current_letter = word[i] - 'A';

    /* if no node, create a new one */
    if (!node->children[current_letter]) {
      node->children[current_letter] = create_node();
    }

    node = node->children[current_letter];
  }

  /* current letter is null, node is still pointing to last letter */
  node->end_of_word = true;

  return true;
}

dictionary_t *create_node(void) {

  /* Question 1 */
  /* get new node, set all pointers to null and end_of_word to false (all 0)*/
  dictionary_t *newnode = (dictionary_t*) malloc(sizeof(dictionary_t));
  newnode->end_of_word = false;
  newnode->children = calloc(ALPHABET_SIZE, sizeof(dictionary_t*));
  return newnode;
}

void free_node(dictionary_t *root) {
  if (root) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      free_node(root->children[i]);
    }
    free(root->children);
    free(root);
  }
}

bool load_from_file(dictionary_t *root, const char *filename) {
  char buff[MAX_WORD_SIZE];

  /* get all words from the file*/
  FILE *file = fopen(filename, "r");

  /* file could not be opened */
  if (!file) {
    return false;
  }

  /* get each line, set last char (newline) to null */
  while (fgets(buff, MAX_WORD_SIZE,file)) {
    insert(root, strtok(buff, "\n"));
  }

  fclose(file);
  return true;
}


