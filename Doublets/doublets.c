#include "doublets.h"
#include <assert.h>

/* ------------------ YOUR CODE HERE ------------------- */

bool valid_step(dictionary_t *dict, const char *curr_word, const char *next_word) {
  /* one character difference*/


  /* check both strings are the same length */
  if (strlen(curr_word) != strlen(next_word)) {
    return false;
  }


  /* go through every character, if a character does not match, and a difference 
  has already been found, return false*/
  bool chardiff = false;

  for (int i = 0; curr_word[i]; i++) {
    if (curr_word[i] != next_word[i]) {
      if (chardiff) {
        return false;
      } else {
        chardiff = true;
      }
    }
  }


  /*if the word is in the glossary, and there is 1 char difference, return true*/
  return find(dict, next_word) && chardiff;

}

void print_case(const char *word, bool upper) {
  for (int i = 0; word[i]; i++) {
    printf("%c", upper?toupper(word[i]):tolower(word[i]));
  }
  printf("\n");
}

void print_chain(const char **chain) {

  assert(chain);

  print_case(chain[0], true);

  int i = 1;
  for (;chain[i + 1];i++) {
    print_case(chain[i], false);
  }

  print_case(chain[i], true);
}

bool valid_chain(dictionary_t *dict, const char **chain) {
  
  /* check each pair of i and i+1 th words */
  for (int i = 0; chain[i] && chain[i+1]; i++) {

    /* valid step*/
    if (!valid_step(dict, chain[i], chain[i+1])) {
      return false;
    }

    /* no repeats */
    for (int j = 0; chain[j]; j++) {
      if (!strcmp(chain[i], chain[j]) && i != j) {
        return false;
      }
    }
  }
  return true;
}

// void add_to_chain(const char *str, int place, const char **chain) {
//   chain[place] = malloc(strlen(str) + 1);
//   strcpy(chain[place], str);
// }

// bool already_used(const char **chain, char* word) {
//   for (int w = 0; chain[w]; w++) {
//     if (!strcmp(chain[w], word)) {
//       return true;
//     }
//   }
//   return false;
// }

// bool find_chain_aux(dictionary_t *dict, const char *start_word, 
//                 const char *target_word, const char **chain, int max_words, int word_no) {
//   if (word_no == max_words) {

//     /* cannot reach words, all out */
//     return false;
//   } else if (valid_step(dict, start_word, target_word)) {

//     /* search is over, have number of words, create chain and start populating*/
//     add_to_chain(target_word, word_no, chain);
//     return true;
//   } else {

//     /* iterate through each character of start_word */
//     for (int c = 0; start_word[c]; c++) {

//       /* iterate through all potential letters at that position */
//       for (char i = 'A'; i <= 'Z'; i++) {

//         /* more efficient way by using subdicts, and finding from word onwards but will ignore for now
//         tail recursion was nice, but could not make an efficient version that checks if a word has been previously used. */
//         char newword[MAX_WORD_SIZE];
//         strcpy(newword, start_word);
//         newword[c] = i;

//         if (!already_used(chain, newword) && i != start_word[c] && find(dict, newword)) {
//           add_to_chain(newword, word_no, chain);

//           if (find_chain_aux(dict, newword, target_word, chain, max_words, word_no + 1)) {
//             return true;
//           }
//         }
//       }
//     }

//     /* no words were found, so a dead end*/
//     return false;
//   }
// }

// bool find_chain(dictionary_t *dict, const char *start_word, 
//                 const char *target_word, const char **chain, int max_words) {
//   add_to_chain(start_word, 0, chain);
//   return find_chain_aux(dict, start_word, target_word, chain, max_words, 1);
// }



//-----

void chainadd(const char *str, int place, const char **chain) {
  chain[place] = strcpy(malloc(strlen(str) + 1), str);
}

bool used(const char **chain, char* word) {
  for (char** w = (char**)chain; *w; w++) {
    if (!strcmp(*w, word)) return true;
  }
  return false;
}

bool find_chain_aux(dictionary_t *dict, const char *start_word, const char *target_word, const char **chain, int max_words, int word_no) {
  if (word_no == max_words) return false;
  else if (valid_step(dict, start_word, target_word)) {
    chainadd(target_word, word_no, chain);
    return true;
  } else {
    for (int c = 0; start_word[c]; c++) {
      for (char i = 'A'; i <= 'Z'; i++) {
        char newword[MAX_WORD_SIZE];
        strcpy(newword, start_word)[c] = i;
        if (!used(chain, newword) && i != start_word[c] && find(dict, newword)) {
          chainadd(newword, word_no, chain);
          if (find_chain_aux(dict, newword, target_word, chain, max_words, word_no + 1)) return true;
        }
      }
    }
    return false;
  }
}

bool find_chain(dictionary_t *dict, const char *start_word, const char *target_word, const char **chain, int max_words) {
  chainadd(start_word, 0, chain);
  return find_chain_aux(dict, start_word, target_word, chain, max_words, 1);
}

/* recursive solution, for a given word, finds one difference words (valid steps), 
recurs through them. When at target, knows how many words in the chain, allocates
 **chain and as recursion goes back down the call stack adds the words*/


