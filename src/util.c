#include "util.h"

int check_is_number(const char *string)
{
    int i;
    
    for (i = 0; i < (int)strlen(string); i++) {
        if (isdigit(string[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
