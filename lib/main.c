#include <stdio.h>
#include "words.h"
#include "score.h"
#include "sound.h"

int main() {
  AudioData audio = initAudio();
  playBackgroundMusic(audio);
  // gets a word, gets user input, checks, loops
  // One giant function
  // That's a lot of stuff. We will want to break this up - Cooper
  check();
  cleanupAudio(audio);
  return 0;
}
