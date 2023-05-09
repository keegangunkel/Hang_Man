#include "cli.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* colors */
#define GRN  "\033[32m"
#define RED  "\033[31m"
#define DFLT "\033[0m"



void clear_screen()
  { system("clear"); }

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

/*
 * Function to load the word bank into a `frame` structure
 * There are at most 7 letters per line, surrounded by a frame
 * @param correct {BITMAP#26} - HIGH prints green
 * @param incorrect {BITMAP#26} - HIGH prints red
 * @return must be free'd with freeFrame
 * https://www.programiz.com/c-programming/examples/display-alphabets
 * https://stackoverflow.com/questions/523724/c-c-check-if-one-bit-is-set-in-i-e-int-variable
 * https://stackoverflow.com/questions/2674312/how-to-append-strings-using-sprintf
*/
Frame* make_word_bank(unsigned correct, unsigned incorrect) {
  const int rows = 6;
  const int letters_per_row = 7;
  const int cols = letters_per_row * 11 + 5; // 11 chars per letter (color codes), 4 chars for padding, 1 char for null
  char matrix[rows][cols];
  memset(matrix, '\0', sizeof(matrix));

  // Draw the outline
  int outline_width = letters_per_row * 2 + 4;
  memset(matrix[0], '-', outline_width);
  memset(matrix[rows-1], '-', outline_width);
  for (int r=1; r<rows-1; r++)
    { sprintf(matrix[r], "| "); }

  int row = 0;
  int col = cols-1;
  for (char c = 'A'; c <= 'Z'; c++) {

    // End the line if letters per row met
    if (!((c - 'A') % letters_per_row)) {
      matrix[row][col] = '|';
      row++;
      col = 2;
    }

    // Set the color, add the character, reset the color
    unsigned char_pos = 1 << (c - 'A');
    if (correct & char_pos)
      { col += sprintf(matrix[row] + col, GRN); }
    else if (incorrect & char_pos)
      { col += sprintf(matrix[row] + col, RED); }
    col += sprintf(matrix[row] + col, "%c " DFLT, c);

  } //for loop

  // Add remaining spaces
  sprintf(matrix[row] + col, "%*c", 25 % letters_per_row + 1, '|'); // Z - A = 25

  Frame* frame = frameFromMatrix(rows, cols, matrix);
  return frame;
}
