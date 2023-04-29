#include <stdio.h>
#include "words.h"

int main() {
  //char* word = getRandomWordStr();
  //Word w = wordFromLetters(word);
  //free(word);

  Word w = getHangmanWord();
  printWord(w);
  freeWord(w);
  return 0;
}
