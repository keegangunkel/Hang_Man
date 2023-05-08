#include "cli.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void clear_screen()
  { system("clear"); }

/* https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c */
char* read_file(const char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "I/O Error opening %s\n", path);
    return NULL;
  }

  // file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buf = malloc(size + 1);
  if (fread(buf, 1, size, file) != (size_t)size) {
    fprintf(stderr, "Error reading file %s\n", path);
    free(buf);
    fclose(file);
    return NULL;
  }

  buf[size] = '\0'; // null terminate if not already
  fclose(file);
  return buf;
}

/* return must be free'd with freeFrame */
Frame* frameFromFile(const char* path) {
  Frame* frame = calloc(sizeof(Frame), 0);
  if (!frame) {
    fprintf(stderr, "Failed to find memory for a frame\n");
    return NULL;
  }
  char* file_content = read_file(path);
  char* line = strtok(file_content, "\n"); // split file into lines

  while (line) {
    int line_len = strlen(line);
    if (line_len > frame->cols) { frame->cols = line_len; }

    // Allocate space
    frame->grid = realloc(frame->grid, (frame->rows+1) * sizeof(char*));
    if (!frame->grid) {
      fprintf(stderr, "Failed to realloc for frame grid\n");
      free(file_content);
      freeFrame(frame);
      return NULL;
    }
    frame->grid[frame->rows] = malloc((line_len+1) * sizeof(char));
    if (!frame->grid[frame->rows]) {
      fprintf(stderr, "Failed to alloc for frame grid at index %d\n", frame->rows);
      free(file_content);
      freeFrame(frame);
      return NULL;
    }
    // Store 'line' in that space
    strncpy(frame->grid[frame->rows], line, line_len);
    frame->grid[frame->rows][line_len] = '\0';
    // Increment for next line
    frame->rows++;
    line = strtok(NULL, "\n");
  }

  free(file_content);
  return frame;
}

void printFrame(Frame* frame) {
  for (int i=0; i<frame->rows; i++) {
    printf("%s\n", frame->grid[i]);
  }
  return;
}

void freeFrame(Frame* frame) {
  for (int i=0; i<frame->rows; i++)
    { free(frame->grid[i]); }
  free(frame->grid);
  free(frame);
  return;
}
