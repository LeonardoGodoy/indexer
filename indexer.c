#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "file_reader.h"
#include "counter.h"

#define true 1
#define false 0

typedef struct relev *Relevance;
struct relev {
 char *file;
 double tf;
 double tfidf;
 int frequency;
};

struct timeval time;
long int start;
long int end;

int node_number = 5000;
Node nodes;

Node get_instance(){
  Node node;
  if (node_number > 4999) {
    node_number = 0;
    nodes = malloc(5000 * sizeof *node);
  }
  node = (nodes + node_number);
  ++node_number;
  return node;
}

Relevance create_relevance(){
  Relevance relevance = malloc(sizeof(*relevance));
  relevance->tf = 0;
  relevance->tfidf = 0;
  relevance->frequency = 0;
  return relevance;
}

Node create_cascade(Node head, char* word, int i, int length){
  char letter = *(word + i);
  Node son = create_node(letter);
  head->son = son;
  if (++i < length) {
    return create_cascade(son, word, i, length);
  } else {
    son->count++;
    return son;
  }
}

Node find_brother(Node node, char letter, int* success){
  Node previous;

  for (node; node; node = node->next){
    previous = node;
    if (node->letter == letter) {
      *success = true;
      return node;
    }
  }
  *success = false;
  return previous;
}

Node find_or_create_brother(Node node, char letter){
  int success;
  Node result = find_brother(node, letter, &success);
  if (success) { return result; }

  Node next = create_node(letter);
  result->next = next;
  return next;
}

Node find_or_create_son(Node node, char letter){
  if(!node->son){ return node->son = create_node(letter); }
  return node->son = find_or_create_brother(node->son, letter);
}

Node add_word(Node head, char* term){
  int i = 0;
  char letter;
  int length = strlen(term);
  Node letter_node = head;

  for (i; i < length; i++) {
    letter = *(term + i);
    letter_node = find_or_create_brother(letter_node, letter);

    if(i == length-1){
      letter_node->count++;
    } else if(!letter_node->son){
      return create_cascade(letter_node, term, ++i, length);
    }
    letter_node = letter_node->son;
  }
  return NULL;
}

void print_tree(Node node){
    Node aux = node->next;
    int i = 0;
    printf("%c: ", node->letter);
    for (aux; aux; aux = aux->next) {
      printf("%c ", aux->letter);
    }
    printf("\n");
}

int count_word(Node head, char* word){
  int i = 0;
  int success = 0;
  int length = strlen(word);
  Node letter_node = head;

  for (i; i < length; i++) {
    char letter = *(word + i);
    if (letter > 64 && letter < 89) {
      letter += 32;
    }

    letter_node = find_brother(letter_node, letter, &success);

    if(!success) { return 0; }
    if(i == length-1) { return letter_node->count; }
    letter_node = letter_node->son;
  }
}

void freedon(Node node){

  if(node->next){
    freedon(node->next);
  }
  if(node->son){
    freedon(node->son);
  }
  free(node);
}

void start_time(){
  gettimeofday (&time, NULL);
  start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void end_time(){
  gettimeofday (&time, NULL);
  end = (time.tv_sec * 1000) + (time.tv_usec / 1000);

  printf ("\nFinalized in %ld milliseconds.\n\n", end - start);
}

void print_options(){
  printf("You have to specify some option\n");
  printf("-f\t--freq N ARQUIVO\n");
  printf("-fw\t--freq-word PALAVRA ARQUIVO\n");
  printf("-s\t--search TERMO ARQUIVO [ARQUIVO ...]\n");
}

int is_selected(char* command, char* option, char* short_option){
  return (!strcmp(command, option) || !strcmp(command, short_option));
}


Node mount(char* file, int* total, Word word){
  long int buff_size = 5000;
  Buffer buffer = create_buffer(file, buff_size);

  Node head = create_node('a');
  Node son;

  if(word) { word->node = head; }

  int i = 0;
  char* term = malloc(1000* sizeof(char));
  int word_count = 0;
  char letter;

  char* content;
  do {
    read_file(buffer);
    content = buffer->content;
    int length = strlen(content);

    for (i = 0; i < length; i++) {
      letter = *(content+i);

      if (letter > 64 && letter < 89) { letter += 32; }
      if (letter > 96 && letter < 123) {
        *(term+word_count) = letter;
        word_count++;

      } else if(letter == ' ') {
        *(term+word_count) = '\0';
        son = add_word(head, term);
        if (son) {
          *total+=1;
          if(word) {
            push_word(word, son);
            word = word->next;
          }
        }
        word_count = 0;
      }
    }
  } while (buffer->cursor < buffer->file_size);

  *(term+word_count) = '\0';
  son = add_word(head, term);
  if (son) {
    *total+=1;
    if(word) {
      push_word(word, son);
      word = word->next;
    }
  }

  free(content);
  return head;
}


void term_frequency(char* term, Relevance relevance){
  int total;
  Node node = mount(relevance->file, &total, NULL);

  relevance->frequency = count_word(node, term);
  freedon(node);
  relevance->tf = (double)relevance->frequency / (double)total;
}

double inverse_frequency(int file_count, int present){
  double div = ((double)file_count / (double)present);
  return log(div);
}

void print_relevances(Relevance relevances[], int count){
  int i = 0;
  Relevance relevance;
  for (i; i < count; i++) {
    relevance = *(relevances+i);
    printf("File %s relevance: %f\n", relevance->file, relevance->tfidf);
  }
}

void freq(int number, char* file){
  start_time();

  int total = 0;
  Word word = create_word(NULL);
  Node node = mount(file, &total, word);

  Word ordered = create_word(word->node);

  Word aux = word;
  while (aux) {
    push_word_ordered(ordered, aux->node, number);
    aux = aux->next;
  }
  print_frequently_words(ordered);

  end_time();
  freedon(node);
}

void freq_word(char* word, char* file){
  start_time();

  int total = 0;
  Node node = mount(file, &total, NULL);
  int c = count_word(node, word);
  printf("Repetitions: %d\n", c);

  end_time();
  freedon(node);
}

void relevance_of_term(char* term, char**files, int file_count){
  start_time();

  int i = 0;
  int count = 0;

  Relevance relevances[file_count];
  Relevance relevance;

  for (i; i < file_count; i++) {
    relevance = create_relevance();
    relevance->file = *(files+i);
    term_frequency(term, relevance);
    if(relevance->frequency){ count++; }
    *(relevances+i) = relevance;
  }

  double idf = inverse_frequency(file_count, count);
  printf("IDF: %f\n", idf);
  for (i = 0; i < file_count; i++) {
    relevance = *(relevances+i);
    relevance->tfidf = relevance->tf * idf;
  }
  end_time();

  print_relevances(relevances, file_count);
}

int main(int argc, char *argv[]){
  if (argc == 1) {
    print_options();
    exit(0);
  }

  char* command = argv[1];

  if (is_selected(command, "--freq", "-f")) {
    int number = strtol(argv[2], NULL, 10);
    char* path = argv[3];
    printf("The %d most frequent words in %s\n", number, path);

    freq(number, path);

  } else if (is_selected(command, "--freq-word", "-fw")) {
    char* w = argv[2];
    char* path = argv[3];

    printf("Repetitions of the word \"%s\"\n", w);
    freq_word(w, path);

  } else if (is_selected(command, "--search", "-s")) {
    char* term = argv[2];
    int file_count = argc-3;

    int i;
    char** files = malloc(file_count * sizeof(char*));
    for (i = 3; i < argc; i++) {
      *(files+i-3) = *(argv+i);
    }

    printf("Searching for \"%s\" \n", term);
    relevance_of_term(term, files, file_count);

  } else {
    printf("Option \"%s\" not available!\n\n", command);
  }

  printf("\nGoodbye o/\n");
}

// # TODO: Create
