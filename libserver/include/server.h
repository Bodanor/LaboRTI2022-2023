#ifndef __SERVER_H__
#define __SERVER_H__

/**
 * @file server.h
 * @author Liwinux & Tinmar1010
 * @brief Main functions declarations used to manage the server requests.
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "sockets.h"
#include "sql_requests.h"

/**
 * @brief Main default settings when no config file is to be found.
 * 
 */
#define DEFAULT_PORT_LISTENING 4444
#define DEFAULT_MAX_THREADS 10
#define MAX_QUEUE 100

/**
 * @brief Server structure that holds mutexes and server settings.
 * 
 */
typedef struct server_t Server_t;

/**
 * @brief Pointer function that will be runned when a user connects. 1 client per thread.
 * 
 * @param p not used.
 * @return void* 
 */
void* ClientFunction(void *p);

/**
 * @brief 
 * 
 * @return -1 if the connection to the database could be established. Otherwise the server socket in returned.
 */
int Server_init(void);

/**
 * @brief Add the client socket to currecly conencted clients sockets which is an array of ints.
 * 
 * @param client_socket the connected client socket to add to the array.
 */
void add_client(int client_socket);

/**
 * @brief Create a new user inside the connected database.
 * 
 * @param username The username to create.
 * @param password The password to addign to this newly created user.
 * @return 0 if the user has successfully been added to the database.
 * @return 1 if the user already exists inside the database.
 * @return -1 if the user could't be created.
 */
int create_new_user(char *username, char *password);

/**
 * @brief Checks if a username passed in parameter is already inside the database.
 * 
 * @param username The username to check.
 * @return 0 : if the username doesn't exist.
 * @return 1 : if the username already exists.
 * @return -1 : if an error occured when retrieving the users from the database.
 */
int username_already_exists(char *username);

/**
 * @brief Checks the provided username and password against the database.
 * 
 * @param username The username to check.
 * @param password The password to check.
 * @return 0: if the username and password matches the database. 
 * @return -1 : if a database error occured or a malloc error occured.
 * @return 1 : if the username doesnt exist in the database.
 * @return 2 : if the password doens't match the database one.
 */
int client_check_creds(char *username, char *password);

int consult(char *idArticle, char *Article);
int articles_already_exists(char *username);

#endif