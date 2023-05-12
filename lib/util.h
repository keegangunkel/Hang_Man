#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>
char upper(char c);
bool is_alphabetic(char c);
bool str_contains(char* str, char c);

bool bit_set(unsigned bm, int pos);
unsigned set_bit(unsigned bm, int pos);

unsigned letter_positions(char* str, char c);
#endif
