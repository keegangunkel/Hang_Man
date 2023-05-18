#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h> // Something to look into? https://github.com/libsdl-org/SDL_mixer -- playwave.c#Mix_PlayChannel

typedef enum {
  BACKGROUND_MUSIC,
  CORRECT_SOUND,
  WRONG_SOUND
} AudioEffect;

typedef struct {
  SDL_AudioDeviceID id;
  Uint8* buffer;
  Uint32 length;
  SDL_AudioSpec wavSpec;
} AudioData;

AudioData loadAudio(AudioEffect a);
void playAudio(AudioData a);
void cleanupAudio(AudioData a);
