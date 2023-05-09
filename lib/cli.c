#include "cli.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* colors */
#define GRN  "\033[32m"
#define RED  "\033[31m"
#define DFLT "\033[0m"

/*
 * Function to print the character bank
 * https://www.programiz.com/c-programming/examples/display-alphabets
 * https://stackoverflow.com/questions/523724/c-c-check-if-one-bit-is-set-in-i-e-int-variable
 * @param correct {unsigned int}:BITMAP#26 - if bit set, print alphabet position green
 * @param incorrect {unsigned int}:BITMAP#26 - if bit set, print alphabet position red
*/
void print_char_bank(unsigned int correct, unsigned int incorrect) {
  for (char c = 'A'; c <= 'Z'; c++) {
    unsigned pos = 1 << (c - 'A');

    if (correct & pos)
      { printf(GRN); }
    else if (incorrect & pos)
      { printf(RED); }
    printf("%c" DFLT, c);
  }
  printf("\n");
  return;
}

/*
There are 7 letters per line
Each `char` is actually a max of 10 and a min of 5
*/
Frame* word_bank(unsigned correct, unsigned incorrect) {
  int rows = 6;
  int cols = 75; // 7 letters per line, max 11 characters for color changing + 4 for padding, 1 for null: 7 * 11 + 4 + 1
  char matrix[rows][cols];

  int dash_count = 7*2+4;
  for (int c=0; c<dash_count; c++) {
    matrix[0][c] = '-';
    matrix[rows-1][c] = '-';
  }
  matrix[0][dash_count] = '\0';
  matrix[rows-1][dash_count] = '\0';
  matrix[1][0] = '|';
  matrix[1][1] = ' ';

  int row = 1;
  int col = 2;
  for (char c = 'A'; c <= 'Z'; c++) {
    if (c == 'H' || c == 'O' || c == 'V') {
      matrix[row][col] = '|';
      matrix[row][col+1] = '\0';
      row++;
      col = 2;
      matrix[row][0] = '|';
      matrix[row][1] = ' ';
    }

    unsigned char_pos = 1 << (c - 'A');
    if (correct & char_pos) {
      matrix[row][col] = '\033';
      matrix[row][col+1] = '[';
      matrix[row][col+2] = '3';
      matrix[row][col+3] = '2';
      matrix[row][col+4] = 'm';
      col += 5;
    }
    else if (incorrect & char_pos) {
      matrix[row][col] = '\033';
      matrix[row][col+1] = '[';
      matrix[row][col+2] = '3';
      matrix[row][col+3] = '1';
      matrix[row][col+4] = 'm';
      col += 5;
    }
    matrix[row][col] = c;
    matrix[row][col+1] = ' ';
    matrix[row][col+2] = '\033';
    matrix[row][col+3] = '[';
    matrix[row][col+4] = '0';
    matrix[row][col+5] = 'm';
    col += 6;
  }

  Frame* frame = frameFromMatrix(rows, cols, matrix);
  return frame;
}

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

/*
 * Generate a frame from a character matrix
 * Return must be free'd with freeFrame 
*/
Frame* frameFromMatrix(int rows, int cols, char matrix[rows][cols]) {
  Frame* frame = calloc(sizeof(Frame), 0);
  if (!frame) {
    fprintf(stderr, "Faile to find memory for matrix frame\n");
    return NULL;
  }

  frame->rows = rows;
  frame->cols = cols;
  frame->grid = calloc(rows, sizeof(char*));
  if (!frame->grid) {
    fprintf(stderr, "Failed to calloc frame grid\n");
    freeFrame(frame);
    return NULL;
  }

  for (int i=0; i<rows; i++) {
    frame->grid[i] = calloc(cols + 1, sizeof(char));
    if (!frame->grid[i]) {
      fprintf(stderr, "Failed to calloc matrix element\n");
      freeFrame(frame);
      return NULL;
    }
    strncpy(frame->grid[i], matrix[i], cols);
  }
  return frame;
}

/* return must be free'd with freeFrame */
Frame* frameFromFile(const char* path) {
  Frame* frame = calloc(sizeof(Frame), 0);
  if (!frame) {
    fprintf(stderr, "Failed to find memory for file frame\n");
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
    /*
    for(int c=0; c<frame->cols; c++) {
      printf("%c = %d\n", frame->grid[i][c], frame->grid[i][c]);
    }
    */
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
