#ifndef SCORE_H
#define SCORE_H

void check();

/* TO DO */
/*
int guessCount(int length, int difficulty);

// printf(scoreboard())?. Shows user the character bank and _ for unguessed letters
// We probably also want to show remaining guesses?
// This is probably out of scope for scoring so @Tony will make the real implementation
// @George just for testing you can just printf("%s || %s\n", guesses, letters);
char* scoreboard(char* guesses, char* letters);

char* userInput(); // return the users guessed characters. Make sure valid input

bool includes(char* string, char c); // check if character is in string


EXAMPLE USAGE
main() {
  word = getHangmanWord();
  int guess_count = guessCount(word.length, EASY);
  char* guessed[26];
  while (guess_count) {
    printf(scoreboard([], word.letters));
    attempt = userInput();
    for (int i=0; attempt[i] != '\0'; i++) {
      if ( attempt[i] in guessed ) { continue; } //use includes()
      if ( attempt[i] in word.letters ) { score += 1; }
    }
  }
  return 0;
}
*/
#endif
