#include <stdio.h>
#include <curl/curl.h> // sudo apt install libcurl4-openssl-dev
// https://youtu.be/x2PaEz1URso

int main() {
  CURL* curl;
  CURLcode resp;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (!curl) { fprintf(stderr, "Failed to initialize curl\n"); return 1; }

  curl_easy_setopt(curl, CURLOPT_URL, "https://random-word-api.herokuapp.com/word?number=1");
  resp = curl_easy_perform(curl);
  if (resp != CURLE_OK) {
    fprintf(stderr, "Failed CURL request: %s\n", curl_easy_strerror(resp));
    curl_easy_cleanup(curl); // do I need to cleanup here?
    return 1;
  }

  printf("%u\n", resp);
  curl_easy_cleanup(curl);
  curl_global_cleanup(); // do I need both cleanups?
  return 0;
}
