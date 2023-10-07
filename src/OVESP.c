#include "OVESP.h"

/**
 * @file OVESP.c
 * @author Liwinux & Tinmar1010
 * @brief MEAN Online VEgetables Shopping Protocol function declarations
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */


/**
 * @brief Convert uint8_t* data to an OVESP structure with rows and columns.
 * Every time a row begins with the @param command, we know we begin a new row.
 * 
 * @param data : The source data to read from.
 * @return OVESP* : Pointer containing the converted data from the @param data pointer.
 * @return NULL : Malloc error occured.
 */
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

/**
 * @brief Convert sql results to a OVESP structure
 * 
 * @param sql_results the sql results to convert.
 * @return OVESP* : Pointer containing the converted data from the sql results.
 * @return NULL : Malloc error occured.
 */
OVESP *OVESP_SQL_TO_OVESP(Sql_result *sql_results, const char *command);

/**
 * @brief Send an OVESP request in form of a string to a socket.
 * 
 * @param request The string to request to send.
 * @param dst_socket The destination socket to send the request to.
 * @return OVESP_DISCONNECT : If the socket has been disconnected. 
 * @return OVESP_INVALID_DATA : If a corrupted data has been passed.
 * @return OVESP_BROKEN_SOCK : If the data could no be received. That doesn't mean that the socket is closed or the connection is broken ! 
 */
static int OVESP_SEND(const char *request, int dst_socket);

/**
 * @brief Login main logic function.
 * 
 * @param request_tokens A pointer to pointer to OVESP struct that will hold all data from the request.
 * @param client_socket The client to try logging in.
 * @return 0 : If the request has successfully been sent.
 * @return OVESP_DISCONNECT : If the socket has been disconnected. 
 * @return OVESP_INVALID_DATA : If a corrupt data has been passed.
 * @return OVESP_BROKEN_SOCK : If the data could no be received. That doesn't mean that the socket is closed or the connection is broken !
 * 
 */
static int OVESP_LOGIN_OPERATION(OVESP *request_tokens, int client_socket);
static int OVESP_ACHAT_OPERATION(OVESP *request_tokens, int client_socket);
/**
 * @brief Receive a OVESP request and create an OVESP structure.
 * 
 * @param reply_tokens A pointer to pointer to OVESP struct that will hold all data from the request.
 * @param dst_socket The source socket to receive a OVESP request.
 * @return 0 : If the request has successfully been sent.
 * @return OVESP_DISCONNECT : If the socket has been disconnected. 
 * @return OVESP_INVALID_DATA : If the data is corruped or a malloc call failed.
 * @return OVESP_BROKEN_SOCK : If the data could no be received. That doesn't mean that the socket is closed or the connection is broken !
 * 
 */
static int OVESP_RECEIVE(OVESP **reply_tokens, int src_socket);

/**
 * @brief Deallocate memory for a pointer to OVESP struct.
 * 
 * @param ovesp The pointer to deallocate memory from.
 */
static void destroy_OVESP(OVESP *ovesp);

// /**
//  * @brief Checks the provided username and password against the database.
//  * 
//  * @param username The username to check.
//  * @param password The password to check.
//  * @return 0: if the username and password matches the database. 
//  * @return -1 : if a database error occured or a malloc error occured.
//  * @return 1 : if the username doesnt exist in the database.
//  * @return 2 : if the password doens't match the database one.
//  */

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
    
    ovesp_result->command = (char*)malloc(sizeof(char)*(strlen(tmp_ptr) + 1));\
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

    res = NULL; 
    tmp_ptr = NULL;

    for (i = 0; i < src_ovsp->rows; i++) { /* Loop through all rows */
        for (j = 0; j <src_ovsp->columns_per_row; j++) { /* Loop through all columns */

            if (res == NULL){ /* If we are at the beginning of the string, we simply allocate for the first token */
                res = (char*)realloc(res, sizeof(char) * (strlen(src_ovsp->data[i][j]) + 2));
                if (res == NULL)
                    return NULL;

                strcpy(res, src_ovsp->data[i][j]);
                strcat(res, "#");
                tmp_ptr = res;
            }
            else{ /* Else, we can now safely use strlen */
                res = (char*)realloc(res, sizeof(char) * (strlen(src_ovsp->data[i][j]) + 2) * strlen(res));
                if (res == NULL)
                    return NULL;

                tmp_ptr = res;
                tmp_ptr = res + strlen(res); /* If we don't do that, we overwrite the beginning of the string ...*/
                strcpy(tmp_ptr, src_ovsp->data[i][j]);

                /* If we are not at the last row -1, we add the #. Else we don't as we don't want a trailing # to be added at the end of the request */
                if (i != src_ovsp->rows -1)
                    strcat(res, "#");
            }
        }
    }

    return res;
}

OVESP *OVESP_SQL_TO_OVESP(Sql_result *sql_results, const char *command)
{
    int i;
    int j;
    OVESP *ovesp_results;

    ovesp_results = (OVESP*)malloc(sizeof(OVESP));
    if (ovesp_results == NULL)
        return NULL;
    
    ovesp_results->rows = sql_results->rows;
    ovesp_results->columns_per_row = sql_results->columns_per_row;

    ovesp_results->command = (char*)malloc(sizeof(char) * strlen(command) + 1);
    if (ovesp_results->command == NULL)
        return NULL;
    
    strcpy(ovesp_results->command, command);

    ovesp_results->data = (char***)malloc(sizeof(char**)*ovesp_results->rows * ovesp_results->columns_per_row);
    if (ovesp_results->data == NULL)
        return NULL;
    
    for (i = 0; i < sql_results->rows; i++) {
        for(j = 0; j < sql_results->columns_per_row; j++) {
            ovesp_results->data[i][j] = (char*)malloc(sizeof(char)*(strlen(sql_results->array_request[i][j]) + 1));
            if (ovesp_results->data[i][j] == NULL)
                return NULL;
            strcpy(ovesp_results->data[i][j], sql_results->array_request[i][j]);
        }
    }

    return ovesp_results;
}
int OVESP_RECEIVE(OVESP **reply_tokens, int src_socket)
{
    int error_check;
    Message *msg;

    msg = NULL;

    if ((error_check = Receive_msg(src_socket, &msg)) < 0)
        return error_check;
    
    *reply_tokens = OVESP_create_results(msg->data);
    /* If an error occured, destroy the message and return */
    if (*reply_tokens == NULL) {
        destroyMessage(msg);
        return -2;
    }

    /* Destroy the message, as we have tokenized it */
    destroyMessage(msg);
    
    return 0;
}
int OVESP_SEND(const char *request, int dst_socket)
{
    int error_check;

    error_check = 0;

    error_check = Send_msg(dst_socket, (uint8_t*)request, strlen(request) + 1);
    if (error_check > 0)
        return 0; /* Return 0 if successfull */

    return error_check;

}
int OVESP_LOGIN_OPERATION(OVESP *request_tokens, int client_socket)
{   
    int error_check;
    char buffer[200];


    error_check = 0;
    /* If new user is asked we check in the database if it already exists */
    if (strcmp(request_tokens->data[0][2], "1") == 0) {
        error_check = sql_create_new_user(request_tokens->data[0][0], request_tokens->data[0][1]);
        printf("%d\n", error_check);
        if (error_check == 1) {
            strcpy(buffer, LOGIN_FAIL(LOGIN_ALREADY_EXISTS));
        }
        else if (error_check == -1) {
            strcpy(buffer, LOGIN_FAIL(LOGIN_DB_FAIL));
        }
        else if (error_check == 0) {
            strcpy(buffer, LOGIN_OK);
        }
    }
    else if (strcmp(request_tokens->data[0][2], "0") == 0) {
        error_check = sql_client_check_creds(request_tokens->data[0][0], request_tokens->data[0][1]);
        if (error_check == 0) {
            strcpy(buffer, LOGIN_OK);
        }
        else if (error_check == 1) {
            strcpy(buffer, LOGIN_FAIL(LOGIN_BAD_USER));
        }
        else if (error_check == 2) {
            strcpy(buffer, LOGIN_FAIL(LOGIN_BAD_PASSWORD));
        }
        else if (error_check == -1) {
            strcpy(buffer, LOGIN_FAIL(LOGIN_DB_FAIL));
        }
    }
    else {
        printf("%s\n", request_tokens->data[0][2]);
        strcpy(buffer, LOGIN_FAIL(LOGIN_BAD_REQUEST));
    }

    return OVESP_SEND(buffer, client_socket);

}
static int OVESP_CONSULT_OPERATION(OVESP *request_tokens, int client_socket)
{
    int error_check;
    char buffer[200];
    Sql_result *result;
    OVESP *tokenized_result;

    if ((error_check = sql_consult(request_tokens->data[0][1], &result)) ==-1) {
         /* Database error */
         strcpy(buffer, CONSULT_FAIL);
         OVESP_SEND(CONSULT_FAIL, client_socket);
         return 1;
    }
    else 
    {
        /* Trouve */
        if ((tokenized_result = OVESP_SQL_TO_OVESP(&result, "CONSULT")) == NULL)
        {
            OVESP_SEND(CONSULT_FAIL, client_socket);
            return 1;
        }
        else
        {
            OVESP_SEND(*(tokenized_result->data), client_socket);
            return 0;
        }   
    }
}
static int OVESP_ACHAT_OPERATION(OVESP *request_tokens, int client_socket)
{
    int error_check;
    char buffer[200];
    Sql_result *result;
    OVESP *tokenized_result;

    if ((error_check = check_articles(request_tokens->data[0][1], request_tokens->data[0][3], &result)) ==-1) {
         /* Database error */
         OVESP_SEND(ACHAT_FAIL, client_socket);
         return 1;
    }
    else if(error_check = 0)
    {
        OVESP_SEND(STOCK_INSUFFISANT, client_socket);
    }
    else 
    {
        /* Trouve */
        if ((tokenized_result = OVESP_SQL_TO_OVESP(&result, "ACHAT")) == NULL)
        {
            OVESP_SEND(ACHAT_FAIL, client_socket);
            return 1;
        }
        else
        {
            OVESP_SEND(*(tokenized_result->data), client_socket);
            return 0;
        }   
    }
}


/**
 * @brief Main server function to handle requests
 * 
 * @param client_socket The client socket to receive requests from
 * @return 0 : If the request has successfully been sent.
 * @return OVESP_DISCONNECT : If the socket has been disconnected. 
 * @return OVESP_INVALID_DATA : If a corrupt data has been passed.
 * @return OVESP_BROKEN_SOCK : If the data could no be received. That doesn't mean that the socket is closed or the connection is broken !
 */
int OVESP_server(int client_socket)
{
    OVESP* request;
    int error_check;

    error_check = 0;

    error_check = OVESP_RECEIVE(&request, client_socket);
    if (error_check < 0)
        return error_check;


    if (strcmp(request->command, LOGIN_COMMAND) == 0) {
        error_check = OVESP_LOGIN_OPERATION(request, client_socket);
        if (error_check < 0) {
            destroy_OVESP(request);
            return error_check;
        }
    }

    if (strcmp(request->command, CONSULT_COMMAND) == 0) {
        error_check = OVESP_CONSULT_OPERATION(request, client_socket);
        if (error_check < 0) {
            destroy_OVESP(request);
            return error_check;
        }
    }
    destroy_OVESP(request);

    return error_check;
}

int OVESP_Login(const char *user, const char *password, const char new_user_flag, int server_socket)
{
    int error_check;
    char buffer[200];
    OVESP *response;
    
    error_check = 0;

    sprintf(buffer, "%s#%s#%s#%c", LOGIN_COMMAND, user, password, new_user_flag + 0x30);

    error_check = OVESP_SEND(buffer, server_socket);
    /* if an error occured we return the return statement from the OVESP_SEND function */
    if (error_check < 0)
        return error_check;
    
    error_check = OVESP_RECEIVE(&response, server_socket);
    if (error_check < 0)
        return error_check;
    


    if (strcmp(response->command, LOGIN_COMMAND) == 0) {
        
        if (strcmp(response->data[0][0], SUCCESS) == 0) {
            destroy_OVESP(response);
            /* The login attempt has succeeded */
            return 0;
        }
        else if (strcmp(response->data[0][0], FAIL) == 0) {
            /* The login attempt has failed. Let's find out why ??? */
            if (strcmp(response->data[0][1], LOGIN_BAD_USER) == 0) {
                destroy_OVESP(response);
                return 1;
            }
            else if (strcmp(response->data[0][1], LOGIN_BAD_PASSWORD) == 0) {
                destroy_OVESP(response);
                return 2;
            }
            else if (strcmp(response->data[0][1], LOGIN_DB_FAIL) == 0) {
                destroy_OVESP(response);
                return 3;
            }
            else if (strcmp(response->data[0][1], LOGIN_ALREADY_EXISTS) == 0) {
                destroy_OVESP(response);
                return 4;
            }
            else {
                destroy_OVESP(response);
                return -4; /* Bad reply from the server */
            }
        }  
        else {
            destroy_OVESP(response);
            return -4; /* Bad reply from the server */
        }
    }
    else {
        destroy_OVESP(response);
        return -4; /* Bad reply from the server */
    }
    
    destroy_OVESP(response);
    return 0;
}


int OVESP_Consult(int idArticle, int server_socket, OVESP *result)
{
    int error_check;
    char buffer[50];
    OVESP *ovesp;
    char *article;
    error_check = 0;

    sprintf(buffer, "%s#%d#", CONSULT_COMMAND, idArticle);


    error_check = OVESP_SEND(buffer, server_socket);
    /* if an error occured we return the return statement from the OVESP_SEND function */
    if (error_check < 0)
        return error_check;


    error_check = OVESP_RECEIVE(&ovesp, server_socket);
    if (error_check < 0)
        return error_check;

    if(strcmp(ovesp->data[0][0], CONSULT_COMMAND) == 0)
    {
        ovesp->data[0][1] = ovesp->data[0][1] + 0x30;
        if(strcmp(ovesp->data[0][1], "-1" ) == 0)
        {
            destroy_OVESP(ovesp);
            return 1;
        }
        else
        {
            /*result = (OVESP *)malloc(sizeof(OVESP));on allouera la mémoire dans le client ?*/
            result = ovesp;    
            destroy_OVESP(ovesp);
            return 0;   
        }
    }
}
int OVESP_Achat(int idArticle, int quantite, int server_socket, OVESP *result)
{
    int error_check;
    char buffer[50];
    OVESP *ovesp;
    char *article;
    error_check = 0;

    sprintf(buffer, "%s#%d#%d#", ACHAT_COMMAND, idArticle, quantite);


    error_check = OVESP_SEND(buffer, server_socket);
    /* if an error occured we return the return statement from the OVESP_SEND function */
    if (error_check < 0)
        return error_check;


    error_check = OVESP_RECEIVE(&ovesp, server_socket);
    if (error_check < 0)
        return error_check;

    if(strcmp(ovesp->data[0][0], ACHAT_COMMAND) == 0)
    {
        ovesp->data[0][1] = ovesp->data[0][1] + 0x30;
        if(strcmp(ovesp->data[0][1], "-1" ) == 0)
        {
            destroy_OVESP(ovesp);
            return 1;
        }
        else if(strcmp(ovesp->data[0][1], "-1" ) == 0)
        {
            destroy_OVESP(ovesp);
            return 0;
        }        
        else
        {
            /*result = (OVESP *)malloc(sizeof(OVESP));on allouera la mémoire dans le client ?*/
            result = ovesp;  
            destroy_OVESP(ovesp);
            return 0;   
        }
    }


}

// }
// OVESP* OVESP_RETRIEVE_TOKENS(char *request, const char *commande)
// {
//     int tokensCount;
//     char *tokens_ptr;
//     OVESP *tokens;
//     int i, j;   


//     tokens = (OVESP*)malloc(sizeof(OVESP));
//     if (tokens == NULL)
//         return NULL;

//     tokens->rows = 0;
//     tokens->columns_per_row = 0;


//     tokens_ptr = request;

//     /* Count how rows and columns are present in the request */
//     while ((tokens_ptr = strchr(tokens_ptr, '#'))!= NULL){
//         if (strcmp(tokens_ptr, commande) == 0) {
//             tokens->rows++;
//             tokens->columns_per_row = 0;
//         }
//         tokens->columns_per_row++;
//     }

//     tokens->data = (char***)malloc(sizeof(char**)*tokens->rows *tokens->columns_per_row);
//     if(tokens->data == NULL)
//         return NULL;    
    
//     /* Initial string request will be modified !!!! */
//     tokens_ptr = strtok(request, "#");
//     tokensCount = 0;

//     for(i = 0;i < tokens->rows;i++)
//     {
//         for(j = 0;j < tokens->columns_per_row;j++)
//         {
//             tokens_ptr = strtok(request, "#");
//             if (strcmp(tokens_ptr, commande)!= 0)
//             {
//                 tokens->data[i][j] = (char*)malloc(strlen(tokens_ptr) + 1);
//                 if (tokens->data[i][j] == NULL)
//                     return NULL;
//                 strcpy(tokens->data[i][j], tokens_ptr);
//             }
//         }
//     }
    
//     return tokens;
// }



// OVESP* OVESP_TOKENIZER(Sql_result **results, char *commande)
// {
//     OVESP *tokens;
//     int i;
//     int j;
//     int x;

    
//     tokens = (OVESP*)malloc(sizeof(OVESP));
//     if (tokens == NULL)
//         return NULL;

//     tokens->tokens = ((*results)->columns_per_row + 1)*((*results)->rows);
//     tokens->tokensData = (char**)malloc(sizeof(char*)*tokens->tokens);
//     if (tokens->tokensData == NULL) {
//         free(tokens); /* Free previously allocated pointer before returning. */
//         return NULL;
//     }
    

//     for(j = 0, x = 0;j<(*results)->rows;j++)
//     {
//         tokens->tokensData[x] = (char*)malloc(sizeof(char)*(strlen(commande) + 2));
//         if (tokens->tokensData[x] == NULL) {
//             free(tokens->tokensData);
//             return NULL;
//         }
//         strcpy(tokens->tokensData[x], commande);
//         strcat(tokens->tokensData[x], "#");
//         x++;
//         for(i=0; i<(*results)->columns_per_row + 1;i++)
//         {
//             tokens->tokensData[x] = (char*)malloc(sizeof(char)*(strlen((*results)->array_request[j][i]) + 2));
//             if (tokens->tokensData[x] == NULL){
//                 free(tokens->tokensData);
//                 return NULL;
//             }

//             strcpy(tokens->tokensData[x], (*results)->array_request[j][i]);
//             strcat(tokens->tokensData[x], "#");
//             x++;
//         }

//         strcpy(tokens->tokensData[x++], DELI); 

//     }

//     return tokens;

// }

void destroy_OVESP(OVESP *ovesp)
{
    int i;
    int j;

    for (i = 0; i < ovesp->rows; i++)
        for (j = 0; j < ovesp->columns_per_row; j++) {
            // TODO : freeeeeeeeee the memory 
        }
}