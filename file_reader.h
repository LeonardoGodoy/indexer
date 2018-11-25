typedef struct buff *Buffer;
struct buff {
   char* file;
   char* content;
   long int size;
   long int cursor;
   long int file_size;
};

Buffer create_buffer(char* file, long int buff_size);

void read_file(Buffer buffer);
