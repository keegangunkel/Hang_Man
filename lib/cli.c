#include "cli.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>

/* colors */
#define GRN  "\033[32m"
#define RED  "\033[31m"
#define DFLT "\033[0m"



void clear_screen()
  { system("clear"); }

void printFrame(Frame* frame) {
  for (int i=0; i<frame->rows; i++)
    { printf("%s\n", frame->grid[i]); }
  return;
}

void debugFrame(Frame* frame) {
  for (int i=0; i<frame->rows; i++) {
    for(int c=0; c<frame->cols; c++) {
      printf("%c = %d\n", frame->grid[i][c], frame->grid[i][c]);
    }
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

/* Function to count characters in string, ignoring escape sequences */
int getVisibleLength(const char* str) {
  int result = 0;
  int escaping = 0;
  const char esc_code = '\x1B';
  const char end_esc_code = '\x6D';

  for (int i=0; str[i] != '\0'; i++) {
    if (esc_code == str[i]) { escaping = 1; }
    if (!escaping) { result += 1; continue; }

    if (end_esc_code == str[i]) { escaping = 0; }
  }
  return result;
}

void addBorderToFrame(Frame* frame, int vpad, int hpad) {
  int outline_width = getVisibleLength(frame->grid[0]) + 2 + 2*hpad;
  int outline_bytes = outline_width * 3;

  const char vert_line[] = { '\xE2', '\x94', '\x82', '\0' };
  const char horz_line[] = { '\xE2', '\x94', '\x80', '\0' };
  const char top_lft[] = "\u250C";
  const char top_rgt[] = "\u2510";
  const char bot_lft[] = "\u2514";
  const char bot_rgt[] = "\u256F";

  // update frame props
  int orig_rows = frame->rows;
  //int orig_cols = frame->cols;
  frame->rows += 2 + 2*vpad;
  frame->cols += 2 + 2*hpad;

  // Setting some new mem
  char** new_grid = malloc(frame->rows * sizeof(char*));
  for (int i=0; i<frame->rows; i++)
    { new_grid[i] = malloc(frame->cols * sizeof(char)); }

  // Fill the frame's top and bottom rows
  for (int i=0; i<outline_bytes-1; i+=3) {
    sprintf(new_grid[0] + i, "%s", horz_line);
    sprintf(new_grid[frame->rows-1] + i, "%s", horz_line);
  }
  char tmp = new_grid[0][3]; // this char will be overwritten by \0 in sprintf
  sprintf(new_grid[0], "%s", top_lft);
  sprintf(new_grid[0] + outline_bytes - 3, "%s", top_rgt);
  sprintf(new_grid[frame->rows - 1], "%s", bot_lft);
  sprintf(new_grid[frame->rows - 1] + outline_bytes - 3, "%s", bot_rgt);
  new_grid[0][3] = tmp;
  new_grid[frame->rows - 1][3] = tmp;

  // Fill the frames new vpad rows
  for (int i=1; i<vpad+1; i++) {
    sprintf(new_grid[i], "%s%*c%s", vert_line, outline_width - 2, ' ', vert_line);
    sprintf(new_grid[frame->rows - i - 1], "%s%*c%s", vert_line, outline_width - 2, ' ', vert_line);
  }

  // Fill in the content rows
  for (int i=0; i<orig_rows; i++) {
    sprintf(new_grid[i+1+vpad], "%s%*s%s%*s%s", vert_line, hpad, "", frame->grid[i], hpad, "", vert_line);
  }

  frame->grid = new_grid;
  return;
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
  /* Configurable vars */
  const int rows = 4;
  const int letters_per_row = 7;
  /* */
  const int cols = letters_per_row * 11; // 11 chars per letter (color codes), 4 chars for padding, 1 char for null
  char matrix[rows][cols];
  memset(matrix, '\0', sizeof(matrix));

  int row = -1;
  int col = 0; // why? good question
  for (char c = 'A'; c <= 'Z'; c++) {

    // End the line if letters per row met
    if (!((c - 'A') % letters_per_row)) {
      matrix[row][col-1] = '\0'; // get rid of the extra space
      row++;
      col = 0;
    }

    // Set the color, add the character, reset the color
    unsigned char_pos = 1 << (c - 'A');
    if (correct & char_pos)
      { col += sprintf(matrix[row] + col, GRN); }
    else if (incorrect & char_pos)
      { col += sprintf(matrix[row] + col, RED); }
    col += sprintf(matrix[row] + col, "%c" DFLT, c);
    col += sprintf(matrix[row] + col, " "); //seperate by space

  } //for loop

  // Add remaining spaces
  sprintf(matrix[row] + col, "%*c", 25 % letters_per_row - 1, ' ');

  Frame* frame = frameFromMatrix(rows, cols, matrix);
  addBorderToFrame(frame, 0, 1);
  return frame;
}
