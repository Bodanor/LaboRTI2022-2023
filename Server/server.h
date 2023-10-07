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
#include "util.h"
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
int Server_init(const int port, const int threads, const char *configfile);

/**
 * @brief Add the client socket to currecly conencted clients sockets which is an array of ints.
 * 
 * @param client_socket the connected client socket to add to the array.
 */
void add_client(int client_socket);

int check_is_number(const char *string);

//int consult(char *idArticle, Sql_result result);
//int articles_already_exists(char *username);


#endif