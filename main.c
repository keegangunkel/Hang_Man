#include <stdio.h>
#include <jansson.h>
#include "http.h"

int main() {
  printf("Random Word Generation example\n");
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


  printf("result: %s\n", word);
  // Now let's get the definition
  
  char dict_endpoint[50];
  sprintf(dict_endpoint, "https://api.dictionaryapi.dev/api/v2/entries/en/%s", word);
  struct ResponseData dict_data = httpGet(dict_endpoint);
  json_t* dict_obj = parse_json(dict_data.data);
  json_t* definition = json_array_get(json_object_get(json_array_get(json_object_get(json_array_get(dict_obj, 0), "meanings"), 0), "definitions"), 0);
  printf("The dict data says: %s\n", json_string_value(json_object_get(definition, "definition")));

  free(dict_data.data);
  json_decref(json_obj); // frees the word variable
  json_decref(dict_obj);
int main() {
  char* word = getRandomWord();
  printf("The random word was %s\n", word);
  free(word);
  return 0;
  
}
