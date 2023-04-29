#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include <jansson.h>


struct ResponseData { char* data; size_t size; CURLcode code; };

size_t http_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
struct ResponseData httpGet(char* url);

json_t* parse_json(const char* json_str);

#endif
