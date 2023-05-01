#include <stdio.h>
#include "words.h"

int main() {
  Word w = getHangmanWord();
  printWord(w);
  freeWord(w);
  return 0;
}
