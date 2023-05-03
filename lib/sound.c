#include <SDL2/SDL.h>
#include <stdio.h>

/* Library functions found here https://wiki.libsdl.org/SDL2/CategoryAPI */

void playBackgroundMusic(SDL_AudioDeviceID deviceId, SDL_AudioSpec* spec, Uint8* buffer, Uint32 length) {
    // Queue and unpause the background music
    SDL_QueueAudio(deviceId, buffer, length);
    SDL_PauseAudioDevice(deviceId, 0);
}

void playCorrectSound(SDL_AudioDeviceID deviceId, SDL_AudioSpec* spec, Uint8* buffer, Uint32 length) {
    // Play sound effect for guessing right
    SDL_QueueAudio(deviceId, buffer, length);
    SDL_PauseAudioDevice(deviceId, 0);
}

void playWrongSound(SDL_AudioDeviceID deviceId, SDL_AudioSpec* spec, Uint8* buffer, Uint32 length) {
    // Play sound effect for guessing wrong
    SDL_QueueAudio(deviceId, buffer, length);
    SDL_PauseAudioDevice(deviceId, 0);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec wavSpec;
    Uint8* bgmBuffer;
    Uint32 bgmLength;

    // Load background music into memory
    SDL_LoadWAV("sounds/background_music.wav", &wavSpec, &bgmBuffer, &bgmLength);
    SDL_AudioDeviceID bgmDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Load sound effects for guessing right and wrong into memory
    Uint8* rightBuffer;
    Uint32 rightLength;
    SDL_LoadWAV("sounds/correct_sound.wav", &wavSpec, &rightBuffer, &rightLength);
    SDL_AudioDeviceID rightDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    Uint8* wrongBuffer;
    Uint32 wrongLength;
    SDL_LoadWAV("sounds/wrong_sound.wav", &wavSpec, &wrongBuffer, &wrongLength);
    SDL_AudioDeviceID wrongDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Play background music
    playBackgroundMusic(bgmDeviceId, &wavSpec, bgmBuffer, bgmLength);

    // Play guessing game with sound effects
    char guess;
    while (1) {
        printf("Guess (c or w): ");
        guess = getchar();

        switch (guess) {
            case 'c':
                playCorrectSound(rightDeviceId, &wavSpec, rightBuffer, rightLength);
                break;
            case 'w':
                playWrongSound(wrongDeviceId, &wavSpec, wrongBuffer, wrongLength);
                break;
            default:
                goto cleanup;
        }

        // Clear input buffer
        while (getchar() != '\n') {
            // do nothing
        }
    }

cleanup:
    // Clean up audio devices and free sound data
    SDL_CloseAudioDevice(bgmDeviceId);
    SDL_FreeWAV(bgmBuffer);

    SDL_CloseAudioDevice(rightDeviceId);
    SDL_FreeWAV(rightBuffer);

    SDL_CloseAudioDevice(wrongDeviceId);
    SDL_FreeWAV(wrongBuffer);

    SDL_Quit();

    return 0;
}