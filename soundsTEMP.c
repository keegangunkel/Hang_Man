
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <mmsystem.h>
// Background music file path
char* bgMusicPath = "background_music.wav";

// Sound effect file paths
char* correctSoundPath = "correct_sound.wav";
char* wrongSoundPath = "wrong_sound.wav";

// Sample function to play sound effect for correct guess
void playCorrectSound() {
    PlaySoundA(correctSoundPath, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP | SND_NODEFAULT);
}

void playWrongSound() {
    PlaySoundA(wrongSoundPath, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP | SND_NODEFAULT);
}
int main() {
    // Play background music on a loop
    PlaySound(bgMusicPath, NULL, SND_FILENAME | SND_ASYNC );

    // Initialize game state variables
    int numGuesses = 0;
    int maxGuesses = 6;
    char word[] = "hangman";
    char guessedLetters[26];
    int numGuessedLetters = 0;

    // Game loop
    while (numGuesses < maxGuesses) {
        // Display current game state
        printf("Word: ");
        for (int i = 0; i < strlen(word); i++) {
            bool letterGuessed = false;
            for (int j = 0; j < numGuessedLetters; j++) {
                if (word[i] == guessedLetters[j]) {
                    printf("%c ", guessedLetters[j]);
                    letterGuessed = true;
                    break;
                }
            }
            if (!letterGuessed) {
                printf("_ ");
            }
        }
        printf("\n");
        printf("Guesses remaining: %d\n", maxGuesses - numGuesses);
        printf("Enter a letter: ");

        // Read user input
        char guess;
        scanf("%c", &guess);
        getchar(); // consume newline character

        // Check if letter has already been guessed
        bool letterAlreadyGuessed = false;
        for (int i = 0; i < numGuessedLetters; i++) {
            if (guess == guessedLetters[i]) {
                printf("You've already guessed that letter!\n");
                letterAlreadyGuessed = true;
                break;
            }
        }

        if (!letterAlreadyGuessed) {
            // Add letter to guessedLetters array
            guessedLetters[numGuessedLetters] = guess;
            numGuessedLetters++;

            // Check if letter is in word
            bool letterFound = false;
            for (int i = 0; i < strlen(word); i++) {
                if (guess == word[i]) {
                    letterFound = true;
                    break;
                }
            }

            // Play appropriate sound effect and update numGuesses
            if (letterFound) {
                playCorrectSound();
            } else {
                playWrongSound();
                numGuesses++;
            }
        }
    }

    // Game over code here...

    return 0;
}
