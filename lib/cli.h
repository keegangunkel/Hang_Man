/*
 * Library for drawing frames in a command line interface
char[] get_guess();
*/

typedef struct {
  int rows;
  int cols;
  char** grid;
} Frame;

void clear_screen();
void printFrame(Frame* frame);
void freeFrame(Frame* frame);

Frame* frameFromFile(const char* path);
Frame* frameFromMatrix(int r, int c, char m[r][c]);
Frame* make_char_bank(unsigned c, unsigned i);

void addBorderToFrame(Frame* frame, int vpad, int hpad);
