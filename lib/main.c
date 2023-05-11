#include <stdio.h>
#include "words.h"
#include "score.h"
#include "sound.h"
#include "cli.h"

void audio_example() {
  AudioData audio = initAudio();
  playBackgroundMusic(audio);
  cleanupAudio(audio);
  return;
}

void cli_example() {
  //clear_screen();
  Frame* snowman = frameFromFile("assets/snowman1.txt");
  Frame* char_bank = make_char_bank(4,8); //67108862
  printFrame(snowman);
  printFrame(char_bank);
  freeFrame(snowman);
  freeFrame(char_bank);
}

void word_example() {
  Word w = getHangmanWord();
  printWord(w);
  freeWord(w);
}

int main() {
  word_example();
  cli_example();
  audio_example();
  return 0;
}

