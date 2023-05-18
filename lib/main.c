#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "words.h"
#include "sound.h"
#include "cli.h"
#include "util.h"

#define DEBUG 1
#define SNOWMEN 8 // the number of snowman assets (starting at 1)

void show_snowman(int guesses, int total) {
  int percentage = (double)guesses / total * 100;
  int snowman_ndx = (percentage / 100.0) * (SNOWMEN-1) + 1;
  if (DEBUG) { printf("Snowman %d guesses / %d total = %d percent == snowman %d\n", guesses, total, percentage, snowman_ndx); }

  char path[20];
  sprintf(path, "assets/snowman%d.txt", snowman_ndx);
  Frame* snowman = frameFromFile(path);
  printAndFreeFrame(snowman);
  return;
}

int calculate_guess_count(Word w)
  { return (26 - uniq_char_count(w.letters)) * 0.3; }

int main() {
  clear_screen();
  // Load the audio files
  AudioData bkgnd = loadAudio(BACKGROUND_MUSIC);
  AudioData correct_effect = loadAudio(CORRECT_SOUND);
  AudioData incorrect_effect = loadAudio(WRONG_SOUND);
  playAudio(bkgnd);

  // initialize the game state
  unsigned   correct = 0; // alphabet bitmap of right guesses
  unsigned incorrect = 0; // alphabet bitmap of wrong guesses
  unsigned   display = 0; // word.letters bitmap of which characters to show the user
  int user_attempts  = 0;

  printf("Searching for a word...\n");
  Word word = getHangmanWord();
  int guess_limit = calculate_guess_count(word);
  printf("Solve the puzzle before the snowman melts!\nEach incorrect guess causes him to melt!\nAfter %d wront attempts he will be gone for the summer!\n", guess_limit);

  // debug stuff
  if (DEBUG) {
    printf("DEBUG: has %d unique letters", uniq_char_count(word.letters));
    printWord(word);
  }

  // While the game is going...
  bool playing = true;
  while (playing) {
    // Display the snowman here?
    show_snowman(user_attempts, guess_limit);
    printAndFreeFrame(make_char_bank(correct, incorrect));
    print_mapped_chars(word.letters, display, " ", "_");
    char user_input[26];
    scanf("%s", user_input);

    int i = 0; // index the user input
    while (user_input[i] != '\0' && playing) {
      if (!is_alphabetic(user_input[i])) { i++; continue; } // skip if non alphabetic
      char input = upper(user_input[i]);
      unsigned input_bit = 1 << (input - 'A');

      // Check if that letter is correct
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
      playing = user_attempts < guess_limit && !high_bitmap(display, strlen(word.letters));
    }

    clear_screen();
    printf("%d remaining attempts\n", guess_limit - user_attempts);
  } //outer while loop

  // Let then know if they won
  show_snowman(user_attempts, guess_limit);
  if (user_attempts < guess_limit) { printf("You guessed the word !  "); }
  else                             { printf("The snowman has melted! "); }
  printWord(word);

  // cleanup
  freeWord(word);
  cleanupAudio(bkgnd);
  cleanupAudio(correct_effect);
  cleanupAudio(incorrect_effect);
  printf("Game Over\n");
  return 0;
}
