#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "words.h"
#include "sound.h"
#include "cli.h"
#include "util.h"

void cli_example() {
  Frame* snowman = frameFromFile("assets/snowman1.txt");
  printFrame(snowman);
  freeFrame(snowman);
}

void print_mapped_chars(char* str, unsigned bm) {
  int i = 0;
  while (str[i] != '\0') {
    if (bit_set(bm, i)) { printf("%c", str[i]); }
    else { printf("_"); }
    i++;
  }
  printf("\n");
  return;
}

int calculate_guess_count(Word w)
  { return (26 - uniq_char_count(w.letters)) * 0.3; }

int main() {
  AudioData bkgnd = loadAudio(BACKGROUND_MUSIC);
  AudioData correct_effect = loadAudio(CORRECT_SOUND);
  AudioData incorrect_effect = loadAudio(WRONG_SOUND);
  playAudio(bkgnd);
  // initialize variables
  unsigned   correct = 0; // alphabet bitmap of right guesses
  unsigned incorrect = 0; // alphabet bitmap of wrong guesses
  unsigned   display = 0; // word.letters bitmap of which characters to show the user
  int user_attempts  = 0;

  // Get a word
  printf("Searching for a word...\n");
  Word word = getHangmanWord();
  int guess_limit = calculate_guess_count(word);
  printf("Solve the puzzle before the snowman melts!\nEach incorrect guess causes him to melt!\nAfter %d wront attempts he will be gone for the summer!\n", guess_limit);

  // debug stuff
  printf("DEBUG: (%d guesses) -> ", guess_limit);
  printf("has %d unique letters", uniq_char_count(word.letters));
  printWord(word);

  // Get their input
  while (user_attempts < guess_limit && !high_bitmap(display, strlen(word.letters))) {
    printAndFreeFrame(make_char_bank(correct, incorrect));
    print_mapped_chars(word.letters, display);
    char user_input[26];
    scanf("%s", user_input);

    int i = 0;
    while (user_input[i] != '\0') {
      if (!is_alphabetic(user_input[i])) { i++; continue; }
      char input = upper(user_input[i]);
      unsigned input_bit = 1 << (input - 'A');

      if (str_contains(word.letters, input)) {
        correct |= input_bit;
        display |= letter_positions(word.letters, input);
        playAudio(correct_effect);
      }
      else {
        // Make sure they don't get docked extra points
        if (!(incorrect & input_bit)) {
          incorrect |= (1 << (input - 'A'));
          user_attempts++;
          playAudio(incorrect_effect);
        }
      }

      i++;
    }

    clear_screen();
  }

  cleanupAudio(bkgnd);
  printf("Game Over\n");
  return 0;
}
