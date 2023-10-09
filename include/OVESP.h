#ifndef OVESP_H
#define OVESP_H
/**
 * @file OVESP.h
 * @author Liwinux & Tinmar1010
 * @brief MEAN Online VEgetables Shopping Protocol function definitions
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "sockets.h"
#include "sql_requests.h"
#include "util.h"

#define MAX_CLIENTS 100

#define LOGIN_COMMAND "LOGIN"
#define SUCCESS "OK"
#define FAIL "KO"
#define CONSULT_COMMAND "CONSULT"
#define ACHAT_COMMAND "ACHAT"
#define CADDIE_COMMAND "CADDIE"
#define CANCEL_COMMAND "CANCEL"
#define CONFIRMER_COMMAND "CONFIRMER"
#define CONFIRMER_SUCCESSFULL "CONFIRMER#1#"
#define CONFIRMER_FAIL "-1"

#define LOGIN_FAIL(REASON) (LOGIN_COMMAND "#" FAIL "#" REASON)
#define LOGIN_OK LOGIN_COMMAND "#" SUCCESS


#define LOGIN_BAD_PASSWORD "BAD_PASS"
#define LOGIN_BAD_USER "BAD_USER"
#define LOGIN_ALREADY_EXISTS "ALREADY_EXISTS"
#define BAD_REQUEST "BAD_REQUEST"
#define SERVER_ERROR "SERVER_ERROR"

#define CONSULT_FAIL "-1"

#define CANCEL_FAIL "-1"

#define ACHAT_FAIL "-1"
#define ACHAT_STOCK_INSUFFISANT "0"

#define CADDIE_FAIL "-1"
#define CADDIE_EMPTY "0"
#define OVESP_DISCONNECT -1
#define OVESP_INVALID_DATA -2
#define OVESP_BROKEN_SOCK -3
#define OVESP_ERROR -4

#define OVESP_DB_FAIL "DB_FAIL"
#define OVESP_BAD_REQUEST "BAD_REQUEST"

typedef struct ovesp_t {
    ssize_t rows;
    ssize_t columns_per_row;
    char *command;
    char ***data;
} OVESP;

/**
 * @brief Main server function to handle requests
 * 
 * @param client_socket The client socket to receive requests from
 * @return 0 : If the request has successfully been sent.
 * @return OVESP_DISCONNECT : If the socket has been disconnected. 
 * @return OVESP_INVALID_DATA : If a corrupt data has been passed.
 * @return OVESP_BROKEN_SOCK : If the data could no be received. That doesn't mean that the socket is closed or the connection is broken !
 */
int OVESP_server(int client_socket, OVESP **caddie);

/* All client functions */

/**
 * @brief Login client attempt main function logic.
 * 
 * @param user The username to login with.
 * @param password The password to login with.
 * @param new_user_flag The flag to create a new user.
 * @param server_socket The server socket to try the login attempt to.
 * 
 * @return 0 : If the username and password are correct and matches an account.
 * @return 1 : If the username doesn't exist.
 * @return 2: If the password is incorrect.
 * @return 3: If there was a database error.
 * @return 4: If the new_user_flag flag has been set and a user already exists under this username.
 * @return -1 : If the connection with the socket is lost. Possibly client or server end.
 * @return -2 : If a malloc error occured or the data is corrupted.
 * @return -3 : If an I/O error occured. That doesn't mean that the socket is closed or the connection is broken !
 * @return -4 : If the server sent a bad reply. Could be a memory error from the server.
 */
int OVESP_Login(const char *user, const char *password, const char new_user_flag, int server_socket);
int OVESP_Consult(int idArticle, int server_socket, OVESP **result);

/**
 * @brief Main function to buy article for client.
 * 
 * @param idArticle The article Id to buy.
 * @param quantite The quantity to buy.
 * @param server_socket The server socket to sent the request to.
 * @param result Pointer allocated containing the updated article after buy.
 * @return 0 : If the function has succeeded.
 * @return 1 : If the article has not been found.
 * @return 2 : If the quantity is too high.
 * @return 3 : If a bad request has been sent to the server.
 * @return -1: Bad response from server.
 */
int OVESP_Achat(int idArticle, int quantite, int server_socket, OVESP **result);
int OVESP_Caddie(int server_socket, OVESP **result);
int OVESP_Cancel(char* idArticle, char *quantity, int server_socket);
int OVESP_Cancel_All(int server_socket, OVESP **caddie);
int OVESP_Confirmer(int server_socket, char *username);
// int OVESP_Logout(int server_socket);



#endif