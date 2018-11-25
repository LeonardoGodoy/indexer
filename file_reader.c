#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_reader.h"

Buffer create_buffer(char* file, long int buff_size){
  Buffer buffer = malloc(sizeof(*buffer));
  buffer->file = file;
  buffer->file_size = 0;
  buffer->cursor = 0L;
  buffer->size = buff_size;
  buffer->content = malloc(sizeof(char) * (buff_size + 1));

  return buffer;
}

void read_file(Buffer buffer){
  FILE *fp = fopen(buffer->file, "r");

  if (fp != NULL) {
    if(buffer->file_size == 0){
      if (fseek(fp, 0L, SEEK_END) == 0) {
        long file_size = ftell(fp);
        if (file_size == -1) { /* Error */ }
        buffer->file_size = file_size;
      }
    }

    if (fseek(fp, buffer->cursor, SEEK_SET) != 0) { /* Error */ }
    size_t newLen = fread(buffer->content, sizeof(char), buffer->size, fp);
    buffer->cursor += buffer->size;

    if (ferror(fp) != 0 ) {
        fputs("Error reading file", stderr);
    } else {
      *(buffer->content + (int)newLen) = '\0';
    }
    fclose(fp);
  }
}
