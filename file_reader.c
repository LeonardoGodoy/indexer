#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "file_reader.h"


Buffer create_buffer(char* file){
  Buffer buffer = malloc(sizeof(*buffer));
  buffer->file = file;
  buffer->size = 0;
  buffer->point = 0;
  return buffer;
}

char* read_file(Buffer buffer, char* path, long int buff_size){
  char *source = NULL;
  FILE *fp = fopen(path, "r");
  if (fp != NULL) {

    if(buffer->size == 0){
      if (fseek(fp, 0L, SEEK_END) == 0) {
        long bufsize = ftell(fp);
        if (bufsize == -1) { /* Error */ }
        buffer->size = bufsize;
        buffer->point = 0L;
      }
    }
      source = malloc(sizeof(char) * (buff_size + 1));
      if (fseek(fp, buffer->point, SEEK_SET) != 0) { /* Error */ }
      size_t newLen = fread(source, sizeof(char), buff_size, fp);
      buffer->point += buff_size;

      if (ferror( fp ) != 0 ) {
          fputs("Error reading file", stderr);
      } else {
        int len = (int)newLen;
        *(source + len) = '\0';
      }

    fclose(fp);
  }

  return source;
}

int example(char* file, long int buff_size){
  Buffer buffer = create_buffer(file);

  char* content;
  do {
    content = read_file(buffer, file, buff_size);
    printf("%s\n", content);
  } while (buffer->point < buffer->size);

  printf("File size: %d\n", buffer->size);
  printf("File point: %d\n", buffer->point);
}
