#include <stdio.h>
#include <curl/curl.h> // sudo apt install libcurl4-openssl-dev
// https://youtu.be/x2PaEz1URso
// // curl.se/libcurl # man libcurl // sudo apt install libcurl4-doc
#include <jansson.h> // sudo apt install libjansson-dev
//https://github.com/akheron/jansson

CURLcode httpGet(char* url) {
  CURL* curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (!curl) { fprintf(stderr, "Failed to initialize curl\n"); return CURLE_FAILED_INIT; }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  res = curl_easy_perform(curl);
  if (res != CURLE_OK)
    { fprintf(stderr, "Failed CURL request: %s\n", curl_easy_strerror(res)); }

  curl_easy_cleanup(curl);
  curl_global_cleanup(); // do I need both cleanups?
  return res;
}

int main() {
  char* word_endpoint = "https://random-word-api.herokuapp.com/word?number=1";
  printf("%u\n", httpGet(word_endpoint));
}
