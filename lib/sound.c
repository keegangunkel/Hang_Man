#include <SDL2/SDL.h>
#include <stdio.h>
#include "sound.h"

/* Library functions found here https://wiki.libsdl.org/SDL2/CategoryAPI */

static int AUDIO_INIT = 0;

AudioData loadAudio(AudioEffect sound) {
  if (!AUDIO_INIT) {
    // I should do this in the http.c --Cooper
    SDL_Init(SDL_INIT_AUDIO);
    AUDIO_INIT = 1;
  }
  AudioData audio = { 0 };
  const char* path;
  switch (sound) {
    case BACKGROUND_MUSIC:
      path = "sounds/background_music.wav";
      break;
    case CORRECT_SOUND:
      path = "sounds/correct_sound.wav";
      break;
    case WRONG_SOUND:
      path = "sounds/wrong_sound.wav";
      break;
  }

  SDL_AudioSpec* I_dont_know_what_this_returns = SDL_LoadWAV(path, &audio.wavSpec, &audio.buffer, &audio.length);
  if (!I_dont_know_what_this_returns) {
    fprintf(stderr, "Failed to load the audio file: %s\n", path);
    exit(1);
  }
  audio.id = SDL_OpenAudioDevice(NULL, 0, &audio.wavSpec, NULL, 0);
  return audio;
}

void playAudio(AudioData audio) {
  SDL_QueueAudio(audio.id, audio.buffer, audio.length);
  SDL_PauseAudioDevice(audio.id, 0);
  return;
}

void cleanupAudio(AudioData audio) {
  SDL_CloseAudioDevice(audio.id);
  SDL_FreeWAV(audio.buffer);
  SDL_Quit();
}
