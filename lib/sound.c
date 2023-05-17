#include <SDL2/SDL.h>
#include <stdio.h>
#include "sound.h"

/* Library functions found here https://wiki.libsdl.org/SDL2/CategoryAPI */

AudioData loadAudioData(const char* path) {
  AudioData audio = { 0 };
  SDL_AudioSpec* I_dont_know_what_this_returns = SDL_LoadWAV(path, &audio.wavSpec, &audio.buffer, &audio.length);
  if (!I_dont_know_what_this_returns) {
    fprintf(stderr, "Failed to load the audio file: %s\n", path);
    exit(1);
  }
  audio.id = SDL_OpenAudioDevice(NULL, 0, &audio.wavSpec, NULL, 0);
  return audio;
}

/*
void playBackgroundMusic(AudioData audio) {
    // Queue and unpause the background music
    SDL_QueueAudio(audio.bgmDeviceId, audio.bgmBuffer, audio.bgmLength);
    SDL_PauseAudioDevice(audio.bgmDeviceId, 0);
} */

void cleanupAudio(AudioData audio) {
  SDL_CloseAudioDevice(audio.id);
  SDL_FreeWAV(audio.buffer);
  // SDL_Quit();
}
