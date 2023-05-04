#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h> // Something to look into? https://github.com/libsdl-org/SDL_mixer -- playwave.c#Mix_PlayChannel

typedef struct {
    SDL_AudioDeviceID bgmDeviceId;
    Uint8* bgmBuffer;
    Uint32 bgmLength;
    SDL_AudioDeviceID rightDeviceId;
    Uint8* rightBuffer;
    Uint32 rightLength;
    SDL_AudioDeviceID wrongDeviceId;
    Uint8* wrongBuffer;
    Uint32 wrongLength;
    SDL_AudioSpec wavSpec;
} AudioData;

AudioData initAudio();
void playBackgroundMusic(AudioData audio);
void playCorrectSound(AudioData audio);
void playWrongSound(AudioData audio);
void cleanupAudio(AudioData audio);
