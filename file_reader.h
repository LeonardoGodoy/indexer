typedef struct buff *Buffer;
struct buff {
   char* file;
   long int size;
   long int point;
};

Buffer create_buffer(char* file);

char* read_file(Buffer buffer, char* path, long int buff_size);

int example(char* file, long int buff_size);
