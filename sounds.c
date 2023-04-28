#include <SDL2/SDL.h>
#include <stdio.h>


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec wavSpec;
    Uint8* bgmBuffer;
    Uint32 bgmLength;

    // Load background music into memory
    SDL_LoadWAV("sounds/background_music.wav", &wavSpec, &bgmBuffer, &bgmLength);
    SDL_AudioDeviceID bgmDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Queue and unpause the background music
    SDL_QueueAudio(bgmDeviceId, bgmBuffer, bgmLength);
    SDL_PauseAudioDevice(bgmDeviceId, 0);

    // Load sound effects for guessing right and wrong into memory
    Uint8* rightBuffer;
    Uint32 rightLength;
    SDL_LoadWAV("sounds/correct_sound.wav", &wavSpec, &rightBuffer, &rightLength);
    SDL_AudioDeviceID rightDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    Uint8* wrongBuffer;
    Uint32 wrongLength;
    SDL_LoadWAV("sounds/wrong_sound.wav", &wavSpec, &wrongBuffer, &wrongLength);
    SDL_AudioDeviceID wrongDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Play guessing game with sound effects
    char guess;
    while (1) {
        printf("Guess (c or w): ");
        guess = getchar();

        switch (guess) {
            case 'c':
                // Play sound effect for guessing right
                SDL_QueueAudio(rightDeviceId, rightBuffer, rightLength);
                SDL_PauseAudioDevice(rightDeviceId, 0);
                break;
            case 'w':
                // Play sound effect for guessing wrong
                SDL_QueueAudio(wrongDeviceId, wrongBuffer, wrongLength);
                SDL_PauseAudioDevice(wrongDeviceId, 0);
                break;
            default:
                return 0;
                break;
        }

        // Clear input buffer
        while (getchar() != '\n') {
            // do nothing
        }
    }

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
