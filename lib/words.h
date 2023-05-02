#ifndef WORDS_H
#define WORDS_H
#include <jansson.h>


typedef struct {
  char* letters;
  char* synonym;
  char* antonym;
  char* definition;
} Word;

void printWord(Word w);
void freeWord(Word w);
char* getRandomWordStr();
json_t* getRandomWordStrs(int count);

Word wordFromLetters(const char* letters);
Word getHangmanWord();
int meetsHangmanRequirements(Word w);

#endif
