#include "ovesp.h"

void destroy_OVESP(OVESP *ovesp)
{
    int i;
    int j;

    free(ovesp->command);
    for (i = 0; i < ovesp->rows; i++) {
        for (j = 0; j < ovesp->columns_per_row; j++) {
            free(ovesp->data[i][j]);
        }
        free(ovesp->data[i]);
        
    }
    free(ovesp->data);
    free(ovesp);
}

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

int OVESP_UPDATE_CADDIE(OVESP *res, OVESP **caddie, const char *command)
{
    int i;
    int j;
    int caddie_size;
    
    caddie_size = 0;
    

    if(strcmp(command, ACHAT_COMMAND) == 0)
    {
        for(i = 0; i < (*caddie)->rows; i++)
        {
            for(j = 0;j < (*caddie)->columns_per_row; j++)
            {
                caddie_size += strlen((*caddie)->data[i][j]) +1;
            }
        }

        (*caddie)->data = (char***)realloc((*caddie)->data, sizeof(char**) *((*caddie)->rows + 1) * (res->columns_per_row * caddie_size));
        if ((*caddie)->data == NULL)
            return -1;
        
        (*caddie)->data[(*caddie)->rows] = (char**)malloc(sizeof(char*) *(res->columns_per_row));
        if ((*caddie)->data[(*caddie)->rows] == NULL)
            return -1;
        
        
        for(i = 0;i<(*caddie)->columns_per_row;i++)
        {
            (*caddie)->data[(*caddie)->rows][i] = (char*)malloc(sizeof(char) * (strlen(res->data[0][i]) + 1));
            if ((*caddie)->data[(*caddie)->rows][i] == NULL)
                return -1;
            strcpy((*caddie)->data[(*caddie)->rows][i], res->data[0][i]);
            
        }
        (*caddie)->rows++;
    }
    else if(strcmp(command, CANCEL_COMMAND) == 0)
    {
        for(i = 0;i<(*caddie)->rows;i++)
        {
            if (strcmp((*caddie)->data[i][0], res->data[0][0]) == 0) {
                for (j = 0; j < (*caddie)->columns_per_row; j++)
                {
                    free((*caddie)->data[i][j]);
                    
                }
                free((*caddie)->data[i]);
                /* Decale tout ce qui se trouve apres*/
                for (j = i ; j < (*caddie)->rows; j++) {
                    if (j + 1 != (*caddie)->rows) {
                        (*caddie)->data[j] =  (*caddie)->data[j+1];
                        (*caddie)->data[j + 1] = NULL;
                    }

                }
                (*caddie)->rows--;
                if ((*caddie)->rows == 0) {
                    destroy_OVESP(*caddie);
                    *caddie = NULL;
                }
                return 0;
            }
        }

        return 1; /* Not found */


    }
        
        return 0;
}