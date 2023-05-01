#include <stdio.h>
#include "words.h"

int check() 
{
    Word word = getHangmanWord();
    printf("%s", word.letters);

    return 0;
}