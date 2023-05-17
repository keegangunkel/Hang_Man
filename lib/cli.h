/*
 * Library for drawing frames in a command line interface
*/
#ifndef CLI_H
#define CLI_H
typedef struct {
  int rows;
  int cols;
  char** grid;
} Frame;

void clear_screen();
void printFrame(Frame* frame);
void freeFrame(Frame* frame);
void printAndFreeFrame(Frame* frame);

Frame* frameFromFile(const char* path);
Frame* frameFromMatrix(int r, int c, char m[r][c]);
// does this make sense to put here?
Frame* make_char_bank(unsigned c, unsigned i); 
void addBorderToFrame(Frame* frame, int vpad, int hpad);
#endif
