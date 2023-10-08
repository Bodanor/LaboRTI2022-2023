#include <stdlib.h>
#include <stdio.h>

#include "ovesp.h"

int main()
{
    OVESP *res;
    OVESP *caddie;

    char buffer[200] = {0};
    char *string;

    string = NULL;
    strcpy(buffer, "ACHAT#1#carottes#4#png.png#ACHAT#2#martin#6#martin.png");
    res = OVESP_create_results((uint8_t*)buffer);
    string = OVESP_TOKENIZER(res);
    printf("%s\n", string);

}