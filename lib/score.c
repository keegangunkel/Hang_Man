#include <stdio.h>
#include "words.h"

int check() 
{
    Word word = getHangmanWord();
    printf("raaaaaaaaaaaaaaaaa   %s", word.letters);
    freeWord(word);

    return 0;
}