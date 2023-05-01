#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include <jansson.h>


typedef struct { char* data; size_t size; CURLcode code; } ResponseData;
typedef struct { char* url; CURL* curl; ResponseData* response; } RequestData;

size_t http_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
void httpGet(RequestData* request);
RequestData* httpInitRequest(char* url);
void freeRequest(RequestData* req);

json_t* parse_json(const char* json_str);

#endif
