#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "words.h"
#include "sound.h"
#include "cli.h"
#include "util.h"

void audio_example() {
  AudioData audio = initAudio();
  playBackgroundMusic(audio);
  cleanupAudio(audio);
  return;
}

void cli_example() {
  //clear_screen();
  Frame* snowman = frameFromFile("assets/snowman1.txt");
  Frame* char_bank = make_char_bank(4,8); //67108862
  printFrame(snowman);
  printFrame(char_bank);
  freeFrame(snowman);
  freeFrame(char_bank);
}

void word_example() {
  Word w = getHangmanWord();
  printWord(w);
  freeWord(w);
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

int main() {
  // initialize variables
  unsigned   correct = 0; // bitmap of right guesses
  unsigned incorrect = 0; // bitmap of wrong guesses
  unsigned   display = 0; // bitmap of which characters to show the user

  Frame* char_bank;

  // Get a word
  Word word = getHangmanWord();
  printf("DEBUG: ");
  printWord(word);

  // Get their input
  while (1) {
    char_bank = make_char_bank(correct, incorrect);
    printFrame(char_bank);
    print_mapped_chars(word.letters, display);
    freeFrame(char_bank);
    char user_input[26];
    scanf("%s", user_input);

    int i = 0;
    while (user_input[i] != '\0') {
      if (!is_alphabetic(user_input[i])) { i++; continue; }
      char input = upper(user_input[i]);

      if (str_contains(word.letters, input)) {
        correct = correct | (1 << (input - 'A'));
        display = display | letter_positions(word.letters, input);
      }
      else
        { incorrect = incorrect | (1 << (input - 'A')); }

      i++;
    }

    break; // testing porpoises
  }

  return 0;
}

/*
void check()
{
    int lives = 3;
    int points = 0;
    float scoreMultiplyer = 1;
    int bonus = 1;

    if (!wordComplete)
    {
        // lives loop keep playing until either you win or run out of lives
        while (lives)
        {
            printf("Enter word: ");
            scanf("%26s", guess);

            int guessLen = strlen(guess);
            // loop going through your guess string
            for (int guessinx = 0; guessinx < guessLen; guessinx++);
            {
                // word loop compares each guess to all letters in the wordt
                for (int wordinx = 0; wordinx < 32; wordinx++)
                {
                    // checks if right
                    // printf(" test %i,    %i \n", guess[guessinx], word.letters[wordinx]);
                    if (guess[guessinx] == word.letters[wordinx])
                    {
                        correct = 1;
                        boolWord[wordinx] = '1';

                        correctGuesses[guessinx] = guess[guessinx];
                        printf("Your smart %d    %s\n", correctCount, boolWord);
                        // printf("Your smart %d    %s\n", correctCount, boolWord);
                    }
                }
                if (correct == 1)
                {
                    int duplicateGuess = strchr(correctGuesses, guess[guessinx]) != NULL;
                    if (duplicateGuess == 0)
                    {
                        boolWord[guessinx] = '1';
                        correctGuesses[guessinx] = guess[guessinx];
                        points = bonus * scoreMultiplyer;
                        // correctCount = correctCount + 1;
                        //  implment score later

                        // printf("Your smart %d    %s\n", correctCount, boolWord);
                    }
                }
                else
                {
                    // checks for redudancy of guesses
                    int duplicateGuess = strchr(wrongGuesses, guess[guessinx]) != NULL;
                    if (duplicateGuess == 0)
                    {
                        printf("You guessed wrong\n");
                        wrongGuesses[guessinx] = guess[guessinx];
                        lives = lives - 1;
                        printf(" %d attempts remain \n", lives);
                    }
                }

                correct = 0;
            }

            if (!lives)
            {
                printf("Game Over\n");
            }

            printf("words are %s    %s \n", constructWord, boolWord);
            boolWord[strlen(boolWord)-1] = '\0';
            if (strcmp(constructWord, boolWord) == -49)
            {
                printf("You completed word (%d pts)\n", points);
                lives = 0;
            }
        }
    }

    printf("_________________________\n");
    freeWord(word);

    return;
    //return boolWord, wrongGuesses, correctGuesses;
}
*/
