#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h> // sudo apt install libcurl4-openssl-dev
// https://youtu.be/x2PaEz1URso
// // curl.se/libcurl # man libcurl // sudo apt install libcurl4-doc
#include <jansson.h> // sudo apt install libjansson-dev
//https://github.com/akheron/jansson
// https://jansson.readthedocs.io/en/2.1/apiref.html#json_t

#include "http.h"

/* Copied from https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html */
size_t http_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  size_t realsize = size * nmemb;
  ResponseData* data = (ResponseData*) userdata;

  // allocate memory
  if (!data->data) {
    data->data = malloc(realsize + 1);
    if (!data->data) {
      fprintf(stderr, "Failed to allocate memory during CURL Request\n");
      return 0;
    }
  }
  // Reallocate the old data from the previous chunk ^
  else {
    char* new_data = realloc(data->data, data->size + realsize + 1);
    if (!new_data) {
      fprintf(stderr, "failed to reallocate memory during CURL Request\n");
      return 0;
    }
    data->data = new_data;
  }

  // Append to data buffer
  memcpy(data->data + data->size, ptr, realsize);
  data->size += realsize;
  data->data[data->size] = '\0';

  return realsize;
}

/* data must be free'd */
ResponseData httpGet(char* url) {
  curl_global_init(CURL_GLOBAL_ALL);
  RequestData request = { .url = url, .curl = curl_easy_init(), .response = NULL };
  ResponseData response = { .data = NULL, .size = 0, .code = 0 };
  request.response = &response;

  if (!request.curl) { fprintf(stderr, "Failed to initialize curl\n"); return response; }

  curl_easy_setopt(request.curl, CURLOPT_URL, request.url);
  curl_easy_setopt(request.curl, CURLOPT_ERRORBUFFER, stderr);
  curl_easy_setopt(request.curl, CURLOPT_WRITEFUNCTION, http_callback);
  curl_easy_setopt(request.curl, CURLOPT_WRITEDATA, request.response);
  request.response->code = curl_easy_perform(request.curl);
  if (request.response->code != CURLE_OK)
    { fprintf(stderr, "Failed CURL request: %s\n", curl_easy_strerror(request.response->code)); }

  curl_easy_cleanup(request.curl);
  curl_global_cleanup(); // do I need both cleanups?
  return response;
}

/* Return must be free'd using json_decref */
/* copied from https://github.com/akheron/jansson/blob/master/examples/simple_parse.c */
json_t* parse_json(const char* json_str) {
  json_error_t error;
  json_t* obj = json_loads(json_str, 0, &error);
  if (!obj) {
    fprintf(stderr, "Failed to parse JSON: %s\n", error.text);
    return NULL;
  }
  return obj;
}

/*
 * EXAMPLE USAGE
int main() {
  char* word_endpoint = "https://random-word-api.herokuapp.com/word?number=1";
  ResponseData word_data = httpGet(word_endpoint);

  json_t* json_obj = parse_json(word_data.data);
  const char *word = json_string_value(json_array_get(json_obj, 0)); // copied from https://stackoverflow.com/questions/47226401/how-to-extract-keyless-values-from-json-array-using-jansson
  free(word_data.data);

  printf("result: %s\n", word);
  json_decref(json_obj);
}
*/
