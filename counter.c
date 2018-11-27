#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counter.h"

Node create_node(char letter, Node dad){
  Node node = malloc(sizeof *node); //get_instance();
  node->letter = letter;
  node->count = 0;
  node->next = NULL;
  node->son = NULL;
  node->dad = dad;
  return node;
}

Node create_initialized_node(char letter, int count){
  Node node = create_node(letter, NULL);
  node->count = count;
  return node;
}

Word create_word(Node node){
  Word word = malloc(sizeof *word);
  word->node = node;
  word->next = NULL;
  return word;
}

void shift(Word word, Node node, int i) {
  Node aux = word->node;
  word->node = node;
  if (word->next){
    shift(word->next, aux, --i);
  } else if(i > 0){
    word->next = create_word(aux);
  }
}

void push_word_ordered(Word word, Node node, int size){
  int i = 0;
  Word previous = word;
  Word iterator = word;

  for (i = size; i > 0; i--) {
    if(!iterator){
      iterator = create_word(node);
      if(previous){ previous->next = iterator; }
      return;
    }
    if(iterator->node->count < node->count){
      shift(iterator, node, --i);
      return;
    }
    previous = iterator;
    iterator = iterator->next;
  }
}

void push_word(Word word, Node node){
  word->next = create_word(node);
}

void print_frequently_words(Word word){
  char* term = malloc(1000 * sizeof(char));
  Node node;
  Word new = word;

  while (new) {
    node = new->node;
    print_word(node, term);
    printf("%s: %d\n", term, node->count);
    new = new->next;
  }
}

void print_word(Node node, char* word){
  int wc = 0;

  while (node) {
    *(word+wc++) = node->letter;
    node = node->dad;
  }
  *(word+wc) = '\0';

  int i = 0;
  for (i = 0; i < wc/2; i++) {
    char letter = *(word+i);
    *(word+i) = *(word+wc-i-1);
    *(word+wc-i-1) = letter;
  }
}

void free_word(Word word){
  while (word) {
    free(word);
    word = word->next;
  }
}
