#include <SDL2/SDL.h>
#include <stdio.h>
#include "sound.h"

/* Library functions found here https://wiki.libsdl.org/SDL2/CategoryAPI */

AudioData initAudio() {
    SDL_Init(SDL_INIT_AUDIO);

    AudioData audio;

    // Load background music into memory
    SDL_LoadWAV("sounds/background_music.wav", &audio.wavSpec, &audio.bgmBuffer, &audio.bgmLength);
    audio.bgmDeviceId = SDL_OpenAudioDevice(NULL, 0, &audio.wavSpec, NULL, 0);

    // Load sound effects for guessing right and wrong into memory
    SDL_LoadWAV("sounds/correct_sound.wav", &audio.wavSpec, &audio.rightBuffer, &audio.rightLength);
    audio.rightDeviceId = SDL_OpenAudioDevice(NULL, 0, &audio.wavSpec, NULL, 0);

    SDL_LoadWAV("sounds/wrong_sound.wav", &audio.wavSpec, &audio.wrongBuffer, &audio.wrongLength);
    audio.wrongDeviceId = SDL_OpenAudioDevice(NULL, 0, &audio.wavSpec, NULL, 0);

    return audio;
}

void playBackgroundMusic(AudioData audio) {
    // Queue and unpause the background music
    SDL_QueueAudio(audio.bgmDeviceId, audio.bgmBuffer, audio.bgmLength);
    SDL_PauseAudioDevice(audio.bgmDeviceId, 0);
}

void playCorrectSound(AudioData audio) {
    // Play sound effect for guessing right
    SDL_QueueAudio(audio.rightDeviceId, audio.rightBuffer, audio.rightLength);
    SDL_PauseAudioDevice(audio.rightDeviceId, 0);
}

void playWrongSound(AudioData audio) {
    // Play sound effect for guessing wrong
    SDL_QueueAudio(audio.wrongDeviceId, audio.wrongBuffer, audio.wrongLength);
    SDL_PauseAudioDevice(audio.wrongDeviceId, 0);
}

void cleanupAudio(AudioData audio) {
    // Clean up audio devices and free sound data
    SDL_CloseAudioDevice(audio.bgmDeviceId);
    SDL_FreeWAV(audio.bgmBuffer);

    SDL_CloseAudioDevice(audio.rightDeviceId);
    SDL_FreeWAV(audio.rightBuffer);

    SDL_CloseAudioDevice(audio.wrongDeviceId);
    SDL_FreeWAV(audio.wrongBuffer);

    SDL_Quit();
}

/*
int main(int argc, char* argv[]) {
    AudioData audio = initAudio();

    // Play background music
    playBackgroundMusic(audio);

    // Play guessing game with sound effects
    char guess;
    while (1) {
        printf("Guess (c or w): ");
        guess = getchar();

        switch (guess) {
            case 'c':
                playCorrectSound(audio);
                break;
            case 'w':
                playWrongSound(audio);
                break;
            default:
                goto end;
        }

        // Clear input buffer
        while (getchar() != '\n') {
            // do nothing
        }
    }

end:
    cleanup(audio);

    return 0;
}
*/
