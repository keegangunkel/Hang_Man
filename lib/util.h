#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>
char upper(char c);
bool is_alphabetic(char c);
bool str_contains(const char* str, char c);
int uniq_char_count(const char* str);

bool bit_set(unsigned bm, int pos);
unsigned set_bit(unsigned bm, int pos);
bool high_bitmap(unsigned bm, int bits);

unsigned letter_positions(const char* str, char c);
#endif
