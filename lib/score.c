#include <stdio.h>
#include "words.h"
#include <string.h>

int check()
{
    Word word = getHangmanWord();
    int wordLen = strlen(word.letters);
    printf("raaaaaaabbbbaaaaaa   %s, %d", word.letters, wordLen);
    printf("\n");

    char guess[26];

    char wrongGuesses[26];
    char correctGuesses[26];
    // bools
    int correct = 0;
    int wordComplete = 0;

    // to be implemented
    int correctCount = 0;

    // change this later
    int lives = 3;
    int points = 0;
    float scoreMultiplyer = 1;
    int bonus = 1;

    if (wordComplete == 0)
    {
        // lives loop keep playing until either you win or run out of lives
        while (0 < lives)
        {
            printf("Enter word: ");
            scanf("%26s", guess);

            int guessLen = strlen(guess);
            // loop going through your guess string
            for (int guessinx = 0; guessinx < guessLen; guessinx++)
            {
                // word loop compares each guess to all letters in the wordt
                for (int wordinx = 0; wordinx < 32; wordinx++)
                {
                    // checks if right
                    if (guess[guessinx] == word.letters[wordinx])
                    {
                        correct = 1;
                    }
                }
                if (correct == 1)
                {
                    int duplicateGuess = strchr(correctGuesses, guess[guessinx]) != NULL;
                    if (duplicateGuess == 0)
                    {
                        correctGuesses[guessinx] = guess[guessinx];

                        correctCount = correctCount + 1;
                        // implment score later
                        points = bonus * scoreMultiplyer;

                        printf("Your smart %d", correctCount);
                        printf("\n");
                    }
                }
                else
                {
                    // checks for redudancy of guesses
                    int duplicateGuess = strchr(wrongGuesses, guess[guessinx]) != NULL;
                    if (duplicateGuess == 0)
                    {
                        printf("You guessed wrong ");
                        wrongGuesses[guessinx] = guess[guessinx];
                        printf("\n");
                        lives = lives - 1;
                        printf(" %d \n", lives);
                    }
                }

                correct = 0;
            }
        }
    }
    if (lives <= 0)
    {
        printf("GameOver");
        printf("\n");
    }
    if (correctCount == wordLen)
    {
        printf("You completed word %d ", points);
        printf("\n");
        lives = -1;
    }

    printf("\n");
    printf("_________________________\n");
    freeWord(word);

    return 0;
}