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

typedef struct ovesp_t {
    long tokens;
    char **tokensData;
} OVESP;


/**
 * @brief Retrieve every token in a request separated by a '#' character.
 * 
 * @param request The request to retrieve tokens from.
 * @return OVESP* : A pointer to a struct containing all the tokens as well a the number of tokens.
 * @return NULL : If a malloc error occured.
 */
static OVESP* OVESP_RETRIEVE_TOKENS(char *request);

/**
 * @brief Send the request to a socket.
 * 
 * @param request The request to send.
 * @param dst_socket The socket to send the request to.
 * @return 0: If successfull.
 * @return -1 : If the socket has been disconnected. 
 * @return -2 : If a bad parameter has been passed to the function.
 * @return -3 : If the data could no be sent. That doesn't mean that the socket is closed or the connection is broken !
 */
static int OVESP_SEND(const char *request, int dst_socket);

/**
 * @brief Main login logic function for a login request.
 * 
 * @param request_tokens Tokens used to perform the connection.
 * @param client_socket The socket used to send a connection status.
 * @return 0: If successfull.
 * @return -1 : If the socket has been disconnected. 
 * @return -2 : If a bad parameter has been passed to the function.
 * @return -3 : If the data could no be sent. That doesn't mean that the socket is closed or the connection is broken !
 */
static int OVESP_LOGIN_OPERATION(OVESP *request_tokens, int client_socket);

/**
 * @brief Receive a OVESP request and tokenize it.
 * 
 * @param reply_tokens A pointer to pointer to OVESP struct that will hold all the tokens from the request.
 * @param dst_socket The source socket to receive a OVESP request.
 * @return 0 : If the request has successfully been sent.
 * @return -1 : If the socket has been disconnected. 
 * @return -2 : If the data is corruped or a malloc call failed.
 * @return -3 : If the data could no be received. That doesn't mean that the socket is closed or the connection is broken !
 * 
 */
static int OVESP_RECEIVE(OVESP **reply_tokens, int src_socket);

/**
 * @brief Deallocate memory for a pointer to OVESP struct.
 * 
 * @param ovesp The pointer to deallocate memory from.
 */
static void destroy_OVESP(OVESP *ovesp);

int OVESP_RECEIVE(OVESP **reply_tokens, int src_socket)
{
    int error_check;
    Message *msg;

    msg = NULL;

    error_check = Receive_msg(src_socket, &msg);
    if (error_check < 0)
        return error_check;
    
    *reply_tokens = OVESP_RETRIEVE_TOKENS((char*)msg->data);
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
void destroy_OVESP(OVESP *ovesp)
{
    int i;

    /* Iterate all the pointers and free them */
    for (i = 0; i < ovesp->tokens; i++)
        free(ovesp->tokensData[i]);

    free(ovesp->tokensData);
    free(ovesp);
}
int OVESP_LOGIN_OPERATION(OVESP *request_tokens, int client_socket)
{   
    int error_check;
    char buffer[200];

    for (error_check = 0; error_check < request_tokens->tokens; error_check++)
        printf("%s\n", request_tokens->tokensData[error_check]);

    error_check = 0;
    /* If new user is asked we check in the database if it already exists */
    if (strcmp(request_tokens->tokensData[3], "1") == 0) {
        error_check = create_new_user(request_tokens->tokensData[1], request_tokens->tokensData[2]);
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
    else if (strcmp(request_tokens->tokensData[3], "0") == 0) {
        error_check = client_check_creds(request_tokens->tokensData[1], request_tokens->tokensData[2]);
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
        printf("%s\n", request_tokens->tokensData[3]);
        strcpy(buffer, LOGIN_FAIL(LOGIN_BAD_REQUEST));
    }

    printf("REPONSE : %s", buffer);
    return OVESP_SEND(buffer, client_socket);

}
OVESP* OVESP_RETRIEVE_TOKENS(char *request)
{
    int tokensCount;
    char *tokens_ptr;
    OVESP *tokens;

    tokensCount = 1;
    tokens_ptr = request;

    /* Count how many tokens are present in the request */
    while ((tokens_ptr = strchr(tokens_ptr, '#'))!= NULL){
        tokensCount++;
        tokens_ptr++;
    }

    /* Allocate memory for the tokens*/
    tokens = (OVESP*)malloc(sizeof(OVESP));
    if (tokens == NULL)
        return NULL;
    
    /* Allocate enough pointers for the tokens */
    tokens->tokens = tokensCount;
    tokens->tokensData = (char**)malloc(sizeof(char*)*tokens->tokens);
    if (tokens->tokensData == NULL) {
        free(tokens); /* Free previously allocated pointer before returning. */
        return NULL;
    }
    
    
    /* Initial string request will be modified !!!! */
    tokens_ptr = strtok(request, "#");
    tokensCount = 0;
    
    /* While all tokens are not retrieved we loop*/
    while (tokens_ptr != NULL) {
        
        /* Allocate memory for the token length + 1 for the null terminator */
        tokens->tokensData[tokensCount] = (char*)malloc(sizeof(char)*strlen(tokens_ptr) + 1);
        if (tokens->tokensData[tokensCount] == NULL) {
            destroy_OVESP(tokens);
            return NULL;
        }
        strcpy(tokens->tokensData[tokensCount++], tokens_ptr); /* Copy the pointer from strktok to the righ token index */
        tokens_ptr = strtok(NULL, "#");
    }
    
    return tokens;
}


int OVESP_server(char *request, int client_socket)
{
    OVESP* tokens;
    int error_check;

    error_check = 0;

    tokens = OVESP_RETRIEVE_TOKENS(request);
    if (tokens == NULL) {
        return -2; /* Server will catch it, maybe sent an error to the client to try again ?*/
    }
    if (strcmp(tokens->tokensData[0], LOGIN_COMMAND) == 0) {
        error_check = OVESP_LOGIN_OPERATION(tokens, client_socket);
    }

    destroy_OVESP(tokens);

    return error_check;
}

int OVESP_Login(const char *user, const char *password, const char new_user_flag, int server_socket)
{
    int error_check;
    char buffer[200];
    OVESP *ovesp;
    
    error_check = 0;

    

    sprintf(buffer, "%s#%s#%s#%c", LOGIN_COMMAND, user, password, new_user_flag + 0x30);

    error_check = OVESP_SEND(buffer, server_socket);
    /* if an error occured we return the return statement from the OVESP_SEND function */
    if (error_check < 0)
        return error_check;
    
    error_check = OVESP_RECEIVE(&ovesp, server_socket);
    if (error_check < 0)
        return error_check;
    


    if (strcmp(ovesp->tokensData[0], LOGIN_COMMAND) == 0) {
        
        if (strcmp(ovesp->tokensData[1], SUCCESS) == 0) {
            destroy_OVESP(ovesp);
            /* The login attempt has succeeded */
            return 0;
        }
        else if (strcmp(ovesp->tokensData[1], FAIL) == 0) {
            /* The login attempt has failed. Let's find out why ??? */
            if (strcmp(ovesp->tokensData[2], LOGIN_BAD_USER) == 0) {
                destroy_OVESP(ovesp);
                return 1;
            }
            else if (strcmp(ovesp->tokensData[2], LOGIN_BAD_PASSWORD) == 0) {
                destroy_OVESP(ovesp);
                return 2;
            }
            else if (strcmp(ovesp->tokensData[2], LOGIN_DB_FAIL) == 0) {
                destroy_OVESP(ovesp);
                return 3;
            }
            else if (strcmp(ovesp->tokensData[2], LOGIN_ALREADY_EXISTS) == 0) {
                destroy_OVESP(ovesp);
                return 4;
            }
            else {
                destroy_OVESP(ovesp);
                return -4; /* Bad reply from the server */
            }
        }  
        else {
            destroy_OVESP(ovesp);
            return -4; /* Bad reply from the server */
        }
    }
    else {
        destroy_OVESP(ovesp);
        return -4; /* Bad reply from the server */
    }
    
    destroy_OVESP(ovesp);
    return 0;
}
int OVESP_Consult(int idArticle, int server_socket)
{
    int error_check;
    char buffer[50];
    OVESP *ovesp;

    error_check = 0;

    sprintf(buffer, "%s#%d#", CONSULT_COMMAND, idArticle);

    error_check = OVESP_SEND(buffer, server_socket);
    /* if an error occured we return the return statement from the OVESP_SEND function */
    if (error_check < 0)
        return error_check;


    error_check = OVESP_RECEIVE(&ovesp, server_socket);
    if (error_check < 0)
        return error_check;

    if(strcmp(ovesp->tokensData[0], CONSULT_COMMAND) == 0)
    {

    }
}