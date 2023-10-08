#include <stdlib.h>
#include <stdio.h>

#include "ovesp.h"

int main()
{
    OVESP *res;
    OVESP *caddie;
    int ret;

    char buffer[200] = {0};

    strcpy(buffer, "ACHAT#2#martin#6#martin.png");
    res = OVESP_create_results((uint8_t*)buffer);
    strcpy(buffer, "ACHAT#2#martin#6#martin.png");
    caddie = OVESP_create_results((uint8_t*)buffer);


    ret = OVESP_UPDATE_CADDIE(res, &caddie, CANCEL_COMMAND);
    destroy_OVESP(res);    
    //destroy_OVESP(caddie);
    //printf("%d\n", ret);

}