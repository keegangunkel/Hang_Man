#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h> // Something to look into? https://github.com/libsdl-org/SDL_mixer -- playwave.c#Mix_PlayChannel

typedef enum {
  BKGND,
  CORRECT,
  WRONG
} SOUND;

char* getSoundPath(SOUND sound);
void playSound(char* path); // non-blocking
