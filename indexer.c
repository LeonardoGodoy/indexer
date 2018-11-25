#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define true 1
#define false 0

typedef struct node *Node;
struct node {
  char letter;
  int count;
  Node next;
  Node son;
 };

struct timeval time;
long int start;
long int end;

int tot = 0;

Node create_node(char letter){
  Node node = malloc(sizeof *node);
  node->letter = letter;
  node->count = 0;
  node->next = NULL;
  node->son = NULL;

  tot++;
  return node;
}

void create_cascade(Node head, char* word, int i, int length){
  char letter = *(word + i);
  Node son = create_node(letter);
  head->son = son;
  if (++i < length) {
    create_cascade(son, word, i, length);
  } else {
    son->count++;
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

void add_word(Node head, char* word){
  int i = 0;
  char letter;
  int length = strlen(word);
  Node letter_node = head;

  for (i; i < length; i++) {
    letter = *(word + i);
    letter_node = find_or_create_brother(letter_node, letter);

    if(i == length-1){
      letter_node->count++;
    } else if(!letter_node->son){
      create_cascade(letter_node, word, ++i, length);
      return;
    }
    letter_node = letter_node->son;
  }
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

void print_word(Node head, char* word){
  int i = 0;
  int success = 0;
  Node letter_node = head;

  for (i; i < strlen(word); i++) {
    char letter = *(word + i);

    letter_node = find_brother(letter_node, letter, &success);
    printf("%d: %c\n", i, letter_node->letter);
    letter_node = letter_node->son;
  }
}

int count_word(Node head, char* word){
  int i = 0;
  int success = 0;
  int length = strlen(word);
  Node letter_node = head;

  for (i; i < length; i++) {
    char letter = *(word + i);
    letter_node = find_brother(letter_node, letter, &success);

    if(!success) { return 0; }
    if(i == length-1) { return letter_node->count; }
    letter_node = letter_node->son;
  }
}


char* read_file(char* path){
  char *source = NULL;
  FILE *fp = fopen(path, "r");
  if (fp != NULL) {
    if (fseek(fp, 0L, SEEK_END) == 0) {
      long bufsize = ftell(fp);
      printf("Long: %d\n", bufsize);
      if (bufsize == -1) { /* Error */ }

      source = malloc(sizeof(char) * (bufsize + 1));
      if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

      size_t newLen = fread(source, sizeof(char), bufsize, fp);
      if ( ferror( fp ) != 0 ) {
          fputs("Error reading file", stderr);
      } else {
        int len = (int)newLen;
        *(source + (--len)) = '\0';
      }
    }
    fclose(fp);
  }

  return source;
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



Node mount(char* file){
  char* buffer = read_file(file);
  int length = strlen(buffer);
  printf("Size: %d\n", length);

  int i = 0;
  int word_count = 0;
  char letter;
  char* word = malloc(40* sizeof(char));

  Node head = create_node('a');
  for (i = 0; i < length; i++) {
    letter = *(buffer+i);

    if (letter > 64 && letter < 89) {
      letter = letter + 32;

    }
    if (letter > 96 && letter < 123) {
      *(word+word_count) = letter;
      word_count++;

    } else if(letter == ' ') {
      *(word+word_count) = '\0';
      add_word(head, word);
      word_count = 0;

    }
  }
  *(word+word_count) = '\0';
  add_word(head, word);
  return head;
}

int is_selected(char* command, char* option, char* short_option){
  return (!strcmp(command, option) || !strcmp(command, short_option));
}


void freq(int number, char* path){
  printf("We are sorry. This is not ready yet.\n");
}

void freq_word(char* word, char* file){
  start_time();

  Node node = mount(file);
  int c = count_word(node, word);
  printf("%s -> %dx\n", word, c);

  end_time();

  freedon(node);
}

void relevance(char* term){ // missing files
  printf("We are sorry. This is not ready yet.\n");
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
    printf("The %d most frequent words in %s\n\n", number, path);

    freq(number, path);

  } else if (is_selected(command, "--freq-word", "-fw")) {
    char* w = argv[2];
    char* path = argv[3];

    printf("Repetitions of the word \"%s\"\n\n", w);
    freq_word(w, path);

  } else if (is_selected(command, "--search", "-s")) {
    char* term = argv[2];
    printf("Search for %s on \n\n", term);

    relevance(term);

  } else {
    printf("Option \"%s\" not available!\n\n", command);
  }

  printf("\nGoodbye o/\n");
}
