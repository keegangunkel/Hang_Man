#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h> // sudo apt install libcurl4-openssl-dev
// https://youtu.be/x2PaEz1URso
// // curl.se/libcurl # man libcurl // sudo apt install libcurl4-doc
#include <jansson.h> // sudo apt install libjansson-dev
//https://github.com/akheron/jansson

struct ResponseData { char* data; size_t size; CURLcode code; };

static size_t http_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  size_t realsize = size * nmemb;
  struct ResponseData* data = (struct ResponseData*) userdata;

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

struct ResponseData httpGet(char* url) {
  CURL* curl;
  struct ResponseData result = { .data = NULL, .size = 0, .code = 0 };

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (!curl) { fprintf(stderr, "Failed to initialize curl\n"); return result; }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, stderr);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
  result.code = curl_easy_perform(curl);
  if (result.code != CURLE_OK)
    { fprintf(stderr, "Failed CURL request: %s\n", curl_easy_strerror(result.code)); }

  curl_easy_cleanup(curl);
  curl_global_cleanup(); // do I need both cleanups?
  return result;
}
}

int main() {
  char* word_endpoint = "https://random-word-api.herokuapp.com/word?number=1";
  struct ResponseData word_data = httpGet(word_endpoint);
  printf("result: %s\n", word_data.data);
}
