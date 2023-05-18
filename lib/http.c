#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "http.h"
#include "common.h"

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


/* Result must be free'd with freeRequest() */
RequestData* httpInitRequest(char* url) {
  curl_global_init(CURL_GLOBAL_ALL); // do I need to call this every time?
  RequestData req = { .url = url, .curl = curl_easy_init(), .response = NULL };
  ResponseData resp = { .data = NULL, .size = 0, .code = 0 };

  // Allocate Request
  RequestData* request = malloc(sizeof(RequestData));
  if (!request) { fprintf(stderr, "Unable to allocate memory for HTTP request\n"); return NULL; }
  
  // Allocate Response
  ResponseData* response = malloc(sizeof(ResponseData));
  req.response = response;
  if (!response) {
    fprintf(stderr, "Unable to allocate memory for HTTP response\n");
    free(request);
    return NULL;
  }
  memcpy(request, &req, sizeof(RequestData));
  memcpy(response, &resp, sizeof(ResponseData));

  curl_easy_setopt(request->curl, CURLOPT_URL, request->url);
  curl_easy_setopt(request->curl, CURLOPT_ERRORBUFFER, stderr);
  curl_easy_setopt(request->curl, CURLOPT_WRITEFUNCTION, http_callback);
  curl_easy_setopt(request->curl, CURLOPT_WRITEDATA, request->response);

  return request;
}

// Fills the response of the request
void httpGet(RequestData* request) {
  request->response->code = curl_easy_perform(request->curl);
  if (request->response->code != CURLE_OK)
    { fprintf(stderr, "Failed CURL request: %s\n", curl_easy_strerror(request->response->code)); }
  return;
}

void freeRequest(RequestData* req) {
  if (!req) { return; }
  ResponseData* resp = req->response;
  if (resp) {
    if (resp->data) { free(resp->data); }
    free(resp);
  }
  if(req->curl)
    { curl_easy_cleanup(req->curl); }
  free(req);
  return;
}

void httpMultiGet(RequestData** requests, int count) {
  if (DEBUG) { printf("Starting a multiget: "); }
  CURLM* multi = curl_multi_init();
  for (int i=0; i<count; i++) {
    curl_multi_add_handle(multi, requests[i]->curl);
  }

  int running = count;
  while (running) {
    // TODO: Check if this returned successfully
    /* CURLMcode code = */ curl_multi_perform(multi, &running);
    if (DEBUG) {printf("."); fflush(stdout); }
  }

  if (DEBUG) { printf("\n"); }
  curl_multi_cleanup(multi);
  return;
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
Example Usage
RequestData* req = httpInitRequest("google.com");
httpGet(req);
freeRequest(req);
*/
