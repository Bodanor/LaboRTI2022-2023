#ifndef __OVESP__
#define __OVESP__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define CONSULT_COMMAND "CONSULT"
#define ACHAT_COMMAND "ACHAT"
#define CADDIE_COMMAND "CADDIE"
#define CANCEL_COMMAND "CANCEL"

typedef struct ovesp_t {
    ssize_t rows;
    ssize_t columns_per_row;
    char *command;
    char ***data;
} OVESP;

OVESP*OVESP_create_results(uint8_t *data);

/**
 * @brief Create a string seperate by a '#' for every token based on the OVESP structure.
 * This function doesn't add a '#' at the end of the string !
 * 
 * @param src_ovsp The source OVESP structure to read from.
 * @return char* : string containing all the data inside the OVESP separated by a '#' for every token.
 * @return NULL : Malloc error.
 */
char *OVESP_TOKENIZER(OVESP *src_ovsp);
int OVESP_UPDATE_CADDIE(OVESP *res, OVESP **caddie, const char *command);
void destroy_OVESP(OVESP *ovesp);
#endif