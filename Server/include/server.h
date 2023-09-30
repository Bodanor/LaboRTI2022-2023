#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "sockets.h"
#include "sql_requests.h"

/* Server Properties */
#define DEFAULT_PORT_LISTENING 4444
#define DEFAULT_MAX_THREADS 10
#define MAX_QUEUE 100

void* ClientFunction(void *p);
int Server_init(void);
void add_client(int client_socket);
int username_already_exists(char *username);
#endif