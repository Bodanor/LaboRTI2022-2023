#include "ovesp.h"

OVESP*OVESP_create_results(uint8_t *data)
{
    OVESP *ovesp_result;
    int i;
    int j;
    char *tmp_ptr;

    ovesp_result = (OVESP*)malloc(sizeof(OVESP));
    if (ovesp_result == NULL)
        return NULL;
    
    ovesp_result->data = NULL;
    tmp_ptr = strtok((char*)data, "#");
    if (tmp_ptr == NULL) /* If not found, bad request*/
        return NULL;
    
    ovesp_result->command = (char*)malloc(sizeof(char)*(strlen(tmp_ptr) + 1));
    if (ovesp_result->command == NULL)
        return NULL;
    
    strcpy(ovesp_result->command, tmp_ptr); /* Copy the command */
    ovesp_result->rows = 0;
    ovesp_result->columns_per_row = 0;

    /* Dont touch this, it works !!!!!!!!!!!!!!!!!! */

    for (i = 0; (tmp_ptr = strtok(NULL, "#")); i++) {
        ovesp_result->data = (char***)realloc(ovesp_result->data, sizeof(char**)*(i + 1));
        if (ovesp_result->data == NULL)
            return NULL;
        ovesp_result->data[i] = NULL;
        for (j = 0; tmp_ptr != NULL && strcmp(ovesp_result->command, tmp_ptr) != 0; tmp_ptr = strtok(NULL, "#"), j++) {
            ovesp_result->data[i] = (char**)realloc(ovesp_result->data[i],sizeof(char*)*(j + 1));
            if (ovesp_result->data[i] == NULL)
                return NULL;
            ovesp_result->data[i][j] = malloc(sizeof(char) * (strlen(tmp_ptr) + 1));
            if (ovesp_result->data[i][j] == NULL)
                return NULL;
            strcpy(ovesp_result->data[i][j], tmp_ptr);
        }
        ovesp_result->columns_per_row = j;
        ovesp_result->rows++;
    }

    return ovesp_result;

}

char *OVESP_TOKENIZER(OVESP *src_ovsp)
{

    
    char *res; /* Final result string */
    char *tmp_ptr; /* Temp variable to avoid overwriting the begining of the string */
    int i; /* Used as index */
    int j; /* Used as index */

    res = (char*)malloc(sizeof(char) *(strlen(src_ovsp->command) + 2));
    if (res == NULL)
        return NULL; 
    strcpy(res, src_ovsp->command);
    strcat(res, "#");
    tmp_ptr = res;


    for (i = 0; i < src_ovsp->rows; i++) { /* Loop through all rows */
        for (j = 0; j <src_ovsp->columns_per_row; j++) { /* Loop through all columns */

            /* Else, we can now safely use strlen */
            res = (char*)realloc(res, sizeof(char) * (strlen(src_ovsp->data[i][j]) + 2) + strlen(res));
            if (res == NULL)
                return NULL;

            tmp_ptr = res;
            tmp_ptr = res + strlen(res); /* If we don't do that, we overwrite the beginning of the string ...*/
            strcpy(tmp_ptr, src_ovsp->data[i][j]);
            strcat(tmp_ptr, "#");
            
        }
        if (src_ovsp->rows > 1 && i != src_ovsp->rows -1) {
            res = (char *)realloc(res, sizeof(char)* (strlen(res) + ((strlen(src_ovsp->command)) + 2)));
            tmp_ptr = res;
            tmp_ptr = res + strlen(res);
            strcat(tmp_ptr, src_ovsp->command);
            strcat(tmp_ptr, "#");
        }    
    }


    return res;
}