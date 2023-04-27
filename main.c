#include <stdio.h>
#include <jansson.h>
#include "http.h"

int main() {
  printf("Random Word Generation example\n");
  char* word_endpoint = "https://random-word-api.herokuapp.com/word?number=1";
  struct ResponseData word_data = httpGet(word_endpoint);

  json_t* json_obj = parse_json(word_data.data);
  const char *word = json_string_value(json_array_get(json_obj, 0)); // copied from https://stackoverflow.com/questions/47226401/how-to-extract-keyless-values-from-json-array-using-jansson
  free(word_data.data);

  printf("result: %s\n", word);
  json_decref(json_obj);
}
