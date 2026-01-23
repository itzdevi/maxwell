#include <stdio.h>
#include <stdlib.h>

char* mw_read_file(const char* filename, unsigned int* sizeOut) {
  FILE* file;
  unsigned char* buffer;
  unsigned int fileSize = 0;
  size_t readSize = 0;

  file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file at path \"%s\"!\n", filename);
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fprintf(stderr, "Failed to seek end of file at path \"%s\"!\n", filename);
    fclose(file);
    return NULL;
  }
  fileSize = ftell(file);
  if (fileSize == -1) {
    fprintf(stderr, "Failed to get file position at path \"%s\"!\n", filename);
    fclose(file);
    return NULL;
  }
  rewind(file);

  buffer = malloc(fileSize + 1);
  if (!buffer) {
    fprintf(stderr, "Failed to allocate memory buffer!\n");
    fclose(file);
    return NULL;
  }

  readSize = fread(buffer, 1, fileSize, file);
  if (readSize != fileSize) {
    fprintf(stderr, "Failed to read file content at path \"%s\"!\n", filename);
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[fileSize] = '\0';

  fclose(file);
  if (sizeOut != NULL) *sizeOut = fileSize;

  return buffer;
}