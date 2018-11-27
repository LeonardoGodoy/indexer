
typedef struct node *Node;
struct node {
  char letter;
  int count;
  Node next;
  Node son;
  Node dad;
 };

typedef struct word *Word;
struct word {
  Node node;
  Word next;
};

Node create_node(char letter, Node node);

Node create_initialized_node(char letter, int count);

Word create_word(Node node);

void shift(Word word, Node node, int i);

void push_word_ordered(Word word, Node node, int size);

void push_word(Word word, Node node);

void print_frequently_words(Word word);

void print_word(Node node, char* word);

void free_word(Word word);
