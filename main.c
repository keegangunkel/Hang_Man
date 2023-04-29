#include <stdio.h>
#include "words.c"

int main() {
  char* word = getRandomWord();
  Word w = wordFromLetters(word);
  free(word);

  printWord(w);
  freeWord(w);
  return 0;
}
