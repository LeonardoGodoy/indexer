#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counter.h"

Node create_node(char letter){
  Node node = malloc(sizeof *node); //get_instance();
  node->letter = letter;
  node->count = 0;
  node->next = NULL;
  node->son = NULL;

  return node;
}

Node create_initialized_node(char letter, int count){
  Node node = create_node(letter);
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
  Node node;
  Word new = word;

  while (new) {
    node = new->node;
    printf("%c: %d\n", node->letter, node->count);
    new = new->next;
  }
}
