/*
 * Library for drawing frames in a command line interface
void clear_screen();
char[] get_guess();
*/

typedef struct {
  int rows;
  int cols;
  char** grid;
} Frame;

Frame* frameFromFile(const char* path);
void printFrame(Frame* frame);
void freeFrame(Frame* frame);
