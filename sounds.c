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

int main() {
    // Play a correct sound
    play_correct_sound();
    
    // Play a wrong sound
    play_wrong_sound();
    
    return 0;
}


