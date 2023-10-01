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
#include "server.h"

#define MAX_CLIENTS 100

int OVESP(char *requete, char *reponse, int socket);

/* All client functions */
int OVESP_Login(const char *user, const char *password, int server_socket);
int OVESP_Consult(int idArticle, int server_socket);
int OVESP_Achat(int idArticle, int quantite, int server_socket);
int OVESP_Caddie(int server_socket);
int OVESP_Cancel(int idArticle, int server_socket);
int OVESP_Cancel_All(int server_socket);
int OVESP_Confirmer(int server_socket);
int OVESP_Logout(int server_socket);


#endif