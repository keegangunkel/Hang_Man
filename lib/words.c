#include <jansson.h>
#include <string.h>
#include "http.h"
#include <setjmp.h>

jmp_buf buffer;

typedef struct {
  char* letters;
  char* synonym;
  char* antonym;
  char* definition;
} Word;


void printWord(Word w) {
  printf("Word(\n.letters = %s,\n.synonym = %s,\n.antonym = %s,\n.definition = %s\n)\n", w.letters, w.synonym, w.antonym, w.definition);
  return;
}

void freeWord(Word w) {
  free(w.letters);
  free(w.synonym);
  free(w.antonym);
  free(w.definition);
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

/*
 * Gets a random word from the internet
 * Result must be free'd
*/
char* getRandomWordStr() {
  RequestData* request = httpInitRequest("https://random-word-api.herokuapp.com/word?number=1");
  httpGet(request);
  json_t* json_obj = parse_json(request->response->data);

  // copied from https://stackoverflow.com/questions/47226401/how-to-extract-keyless-values-from-json-array-using-jansson
  const char *word = json_string_value(json_array_get(json_obj, 0)); 
  char* result = malloc(strlen(word) + 1);
  strcpy(result, word);

  freeRequest(request);
  json_decref(json_obj);
  return result;
}

/*
 * Gets a bunch of words from the API
 * Result must be freed with json_decref
 * @param int - The amount of words you want to return
 * @return json_decref
*/
json_t* getRandomWordStrs(int count) {
  char word_endpoint[100];
  sprintf(word_endpoint, "https://random-word-api.herokuapp.com/word?number=%d", count);
  RequestData* request = httpInitRequest(word_endpoint);
  httpGet(request);
  json_t* result = parse_json(request->response->data);
  freeRequest(request);
  return result;
}

/* Must be freed with freeWord() */
Word wordFromJson(json_t* json) {
  Word result = { 0 };
  const char* err = json_string_value(json_object_get(json, "title"));
  if (err != NULL)
    { return result; }

  json_t* word_json = json_array_get(
    json_object_get(
      json_array_get(
        json, 0),
      "meanings"),
    0);

  const char* letters = json_string_value(json_object_get(json_array_get(json, 0), "word"));
  const char* syn_str = json_string_value(json_array_get(json_object_get(word_json, "synonyms"), 0));
  const char* ant_str = json_string_value(json_array_get(json_object_get(word_json, "antonyms"), 0));
  const char* def_str = json_string_value(json_object_get(json_array_get(json_object_get(word_json, "definitions"), 0), "definition"));

  charPtrCpy(&result.letters,    letters);
  charPtrCpy(&result.synonym,    syn_str);
  charPtrCpy(&result.antonym,    ant_str);
  charPtrCpy(&result.definition, def_str);

  return result;
}

/* Must be freed with freeWord() */
Word wordFromLetters(const char* letters) {
  Word result = { 0 };
  char dict_endpoint[100];
  sprintf(dict_endpoint, "https://api.dictionaryapi.dev/api/v2/entries/en/%s", letters);

  RequestData* request = httpInitRequest(dict_endpoint);
  httpGet(request);
  json_t* dict_obj = parse_json(request->response->data);
  freeRequest(request);
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

/* Does this word meet the requirements? */
int meetsHangmanRequirements(Word w) {
  if (w.letters == NULL) { return 0; }
  if (w.synonym == NULL) { return 0; }
  if (w.antonym == NULL) { return 0; }
  if (w.definition == NULL) { return 0; }
  return 1;
}

/*
 * The only function to be called from main
 * result must be free'd with freeWord
*/
Word getHangmanWord() {
  Word result = { 0 };
  const int word_count = 50;
  // Get a list of word strings
  json_t* json_words = getRandomWordStrs(word_count);

  // Setup the dictionary requests
  RequestData* requests[word_count];
  for (int i=0; i<word_count; i++) {
    char endpoint[100];
    const char* letters = json_string_value(json_array_get(json_words, i));
    sprintf(endpoint, "https://api.dictionaryapi.dev/api/v2/entries/en/%s", letters);
    requests[i] = httpInitRequest(endpoint);
  }

  // Perform the requests
  httpMultiGet(requests, word_count);

  // Pick a word out of those requests
  int word_ndx = 0;
  for (word_ndx=0; word_ndx<word_count; word_ndx++) {
    json_t* json = parse_json(requests[word_ndx]->response->data);
    if (!json) {
      fprintf(stderr, "You're probably being rate limited, shutting down\n");
      longjmp(buffer, 1);
    }

    result = wordFromJson(json);
    json_decref(json);

    if (meetsHangmanRequirements(result)) { break; }
    freeWord(result);
    freeRequest(requests[word_ndx]);
  }

  // Free the rest of the unused requests
  int fre = word_ndx + 1;
  for (int i=fre; i<word_count; i++)
    { freeRequest(requests[i]); }

  if (word_ndx == word_count) { return getHangmanWord(); }
  return result;
}
