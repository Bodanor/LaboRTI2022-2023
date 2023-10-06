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

#define MAX_CLIENTS 100

#define LOGIN_COMMAND "LOGIN"
#define SUCCESS "OK"
#define FAIL "KO"
#define CONSULT_COMMAND "CONSULT"

#define LOGIN_FAIL(REASON) (LOGIN_COMMAND"#"FAIL"#"REASON)
#define LOGIN_OK LOGIN_COMMAND"#"SUCCESS


#define LOGIN_BAD_PASSWORD "BAD_PASS"
#define LOGIN_BAD_USER "BAD_USER"
#define LOGIN_DB_FAIL "DB_FAIL"
#define LOGIN_ALREADY_EXISTS "ALREADY_EXISTS"
#define LOGIN_BAD_REQUEST "BAD_REQUEST"
#define SERVER_ERROR "SERVER_ERROR"

#define CONSULT_FAIL "CONSULT#FAIL"
#define DELI ":-)"


typedef struct ovesp_t {
    ssize_t rows;
    ssize_t columns_per_row;
    char ***data;
} OVESP;


int OVESP_server(int client_socket);

/* All client functions */

/**
 * @brief Login attempt main function logic.
 * 
 * @param user The username to login with.
 * @param password The password to login with.
 * @param new_user_flag The flag to create a new user.
 * @param server_socket The server socket to try the attempt to.
 * @return 0 : If the username and password are correct and matches an account.
 * @return 1 : If the username doesn't exist.
 * @return 2: If the password is incorrect.
 * @return 3: If there was a database error.
 * @return 4: If the @param new_user_flag flag has been set and a user already exists under this username.
 * @return -1 : If the connection with the socket is lost. Possibly client or server end.
 * @return -2 : If a malloc error occured or the data is corrupted.
 * @return -3 : If an I/O error occured. That doesn't mean that the socket is closed or the connection is broken !
 * @return -4 : If the server sent a bad reply. Could be a memory error from the server.
 */
int OVESP_Login(const char *user, const char *password, const char new_user_flag, int server_socket);
int OVESP_Consult(int idArticle, int server_socket, OVESP *result);
int OVESP_Achat(int idArticle, int quantite, int server_socket);
int OVESP_Caddie(int server_socket);
int OVESP_Cancel(int idArticle, int server_socket);
int OVESP_Cancel_All(int server_socket);
int OVESP_Confirmer(int server_socket);
int OVESP_Logout(int server_socket);


#endif