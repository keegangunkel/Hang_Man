#include "util.h"


/* String / Char functions */
char upper(char c) {
  if (c >= 'a' && c <= 'z')
    { c = c - 32; }
  return c;
}

bool is_alphabetic(char c) {
  c = upper(c);
  if (c >= 'A' && c <= 'Z') { return true; }
  return false;
}


bool str_contains(char* str, char c) {
  c = upper(c);
  int i = 0;
  while (str[i] != '\0') {
    if (upper(str[i]) == c) { return true; }
    i++;
  }
  return false;
}

/* Bitwise functions */

bool bit_set(unsigned bm, int pos) {
  unsigned mask = 1 << pos;
  return (bm & mask) != 0;
}

unsigned set_bit(unsigned bm, int pos) {
  unsigned mask = 1 << pos;
  return bm | mask;
}

/*
 * Function to find what positions a char is in a string
 * @return bitmap
*/
unsigned letter_positions(char* str, char c) {
  c = upper(c);
  int i = 0;
  unsigned result = 0;
  while (str[i] != '\0') {
    if (upper(str[i]) == c)
    { result = set_bit(result, i); }
    i++;
  }
  return result;
}


