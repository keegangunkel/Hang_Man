#include <stdio.h>
#include <jansson.h>
#include <SDL2/SDL.h> // sudo apt install libsdl2-dev
#include <SDL2/SDL_audio.h> // included with ^
#include <SDL2/SDL_mixer.h> // sudo apt install libsdl2-mixer-dev
#include "http.h"

typedef struct {
  char* letters;
  char* synonym;
  char* antonym;
  char* definition;
} Word;

/*
 * Gets a random word from the internet
 * Result must be free'd
*/
char* getRandomWord() {
  char* word_endpoint = "https://random-word-api.herokuapp.com/word?number=1";
  struct ResponseData word_data = httpGet(word_endpoint);
  json_t* json_obj = parse_json(word_data.data);

  // copied from https://stackoverflow.com/questions/47226401/how-to-extract-keyless-values-from-json-array-using-jansson
  const char *word = json_string_value(json_array_get(json_obj, 0)); 
  char* result = malloc(strlen(word) + 1);
  strcpy(result, word);

  free(word_data.data);
  json_decref(json_obj);
  return result;
}

/*
 * Copies string from one pointer to another
 * @param dest - the destination to copy to. To malloc this needs to be a pointer to a pointer.
 * @param src - The source string to copy into the destination
*/
void charPtrCpy(char** dest, const char* src) {
  if (src == NULL) { return; }
  *dest = malloc(strlen(src) + 1);
  if (*dest == NULL) { 
    fprintf(stderr, "Failed to allocate memory\n");
    return;
  }
  strcpy(*dest, src);
  return;
}

Word wordFromLetters(char* letters) {
  Word result = { 0 };
  char dict_endpoint[100];
  sprintf(dict_endpoint, "https://api.dictionaryapi.dev/api/v2/entries/en/%s", letters);

  struct ResponseData dict_data = httpGet(dict_endpoint);
  json_t* dict_obj = parse_json(dict_data.data);
  free(dict_data.data);
  const char* err = json_string_value(json_object_get(dict_obj, "title"));
  if (err != NULL) {
    // return early if error present
    fprintf(stderr, "DEBUG: %s not found in dict\n", letters);
    json_decref(dict_obj);
    return result;
  }

  json_t* word_json = json_array_get(
    json_object_get(
      json_array_get(
        dict_obj, 0),
      "meanings"),
    0);

  const char* syn_str = json_string_value(json_array_get(json_object_get(word_json, "synonyms"), 0));
  const char* ant_str = json_string_value(json_array_get(json_object_get(word_json, "antonyms"), 0));
  const char* def_str = json_string_value(json_object_get(json_array_get(json_object_get(word_json, "definitions"), 0), "definition"));

  charPtrCpy(&result.letters,    letters);
  charPtrCpy(&result.synonym,    syn_str);
  charPtrCpy(&result.antonym,    ant_str);
  charPtrCpy(&result.definition, def_str);

  json_decref(dict_obj);
  return result;
}

void printWord(Word w) {
  printf("Word(\n.letters = %s,\n.synonym = %s,\n.antonym = %s,\n.definition = %s\n)\n", w.letters, w.synonym, w.antonym, w.definition);
  return;
}

/* Library functions found here https://wiki.libsdl.org/SDL2/CategoryAPI */
void playWavFile(char* path) {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
    return;
  }
  Mix_Chunk* sound = Mix_LoadWAV(path);
  if (!sound) {
    fprintf(stderr, "Mix_LoadWAV failed: %s\n", Mix_GetError());
    return;
  }
  if (Mix_PlayChannel(-1, sound, 0) < 0) {
    printf("Mix_PlayChannel failed: %s\n", Mix_GetError());
    return;
  }

  while (Mix_Playing(-1)) { SDL_Delay(100); } // do we need this?
  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
  return;
} //playSoundFile

int main() {
  char* word = getRandomWord();
  Word w = wordFromLetters(word);
  free(word);

  printWord(w);
  playWavFile("sounds/correct_sound.wav");
  return 0;
}
