#include "util.h"
#include <stdio.h>


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

/* Not case sensitive */
bool str_contains(const char* str, char c) {
  c = upper(c);
  int i = 0;
  while (str[i] != '\0') {
    if (upper(str[i]) == c) { return true; }
    i++;
  }
  return false;
}

int uniq_char_count(const char* str) {
  bool alpha[26] = { false };
  int result = 0;
  int i = 0;
  while (str[i] != '\0') {
    int pos = upper(str[i]) - 'A';
    if (alpha[pos]) { i++; continue; }

    alpha[pos] = true;
    result++;
    i++;
  }
  return result;
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

/* Is every bit turned on? */
bool high_bitmap(unsigned bm, int bits) {
  if (bm == 0) { return false; }
  for (int i=0; i<bits; i++) {
    if (!(bm & 1)) { return false; }
    bm >>= 1;
  }
  return true;
}

/*
 * Function to find what positions a char is in a string
 * @return bitmap
*/
unsigned letter_positions(const char* str, char c) {
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


