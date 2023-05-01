#include <stdio.h>
#include "words.h"
#include <string.h>


int check()
{
    Word word = getHangmanWord();
    printf("raaaaaaabbbbaaaaaa   %s", word.letters);
    printf("\n");

    char guess[26];

    printf("Enter word: ");
    scanf("%26s", guess);

    printf("you pailed: %s\n", guess);
    int guessLen = strlen(guess);

    char wrongGuesses[26];
    char correctGuesses[26];

    int correct = 0;
    // change this later
    int lives = 3;
    for (int guessinx = 0; guessinx < guessLen; guessinx++)
    {
        for (int wordinx = 0; wordinx < 32; wordinx++)
        {
            if (guess[guessinx] == word.letters[wordinx])
            {
                correct = 1;
            }
        }
        if (correct == 1)
        {
            printf("Your smart");
            correctGuesses[guessinx] = guess[guessinx];
            printf("\n");
        }
        else
        {   
            
            
            printf("You guessed wrong ");
            wrongGuesses[guessinx] = guess[guessinx];
            printf("\n");
        }
        correct = 0;
        

    }

    printf("\n");

    freeWord(word);

    return 0;
}