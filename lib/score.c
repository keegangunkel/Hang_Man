#include <stdio.h>
#include "words.h"
#include <string.h>


/*
check()
{
    // first one is letters of truth in word second is wrong and third is correct
    //char wordData[3][26]; //unused? -Cooper

    Word word = getHangmanWord();
    int wordLen = strlen(word.letters);
    printf("DEBUG: %s (%d)\n", word.letters, wordLen);

    char guess[26];
    char wrongGuesses[26];
    char correctGuesses[26];
    // bools
    int correct = 0;
    int wordComplete = 0;

    // will turn off or on on right answers
    char boolWord[wordLen];
    char constructWord[wordLen];
    for (int i = 0; i < wordLen - 1; i++)
    {
        boolWord[i] = '0';
        constructWord[i] = '1';
    }

    //

    // to be implemented
    int correctCount = 0;

    // change this later
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
            for (int guessinx = 0; guessinx < guessLen; guessinx++)
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
            if (atoi(constructWord) == atoi(boolWord))
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