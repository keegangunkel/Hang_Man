#include <Windows.h>
#pragma comment(lib, "winmm.lib")


void play_sound(const char* filename) {
    PlaySound(filename, NULL, SND_FILENAME);
}

void play_correct_sound() {
    const char* filename = "correct_sound.wav";
    play_sound(filename);
}

void play_wrong_sound() {
    const char* filename = "wrong_sound.wav";
    play_sound(filename);
}


void play_background_music(const char* filename) {
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

int main() {
    // Start playing the background music in a separate thread
    const char* bg_music_filename = "bg_music.wav";
    PlaySound(bg_music_filename, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

// Below is hypothetical code so the sounds can be implemented it will most likely come into play when george codes the correct
// and wrong guesses

/*
    // Game loop
    while (1) {
        // Get user input and update the game state

        // If the user makes a correct guess, play the correct sound
        if (user_makes_correct_guess()) {
            play_correct_sound();
        }
        // If the user makes a wrong guess, play the wrong sound
        else if (user_makes_wrong_guess()) {
            play_wrong_sound();
        }

        // Check if the game is over
        if (game_is_over()) {
            break;
        }
    }
*/
    return 0;
}

