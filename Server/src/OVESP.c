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

static int OVESP_SEND(char *requete, int server_socket);
static int OVESP_RECEIVE(char **reply, int server_socket);


int OVESP_SEND(char *requete, int server_socket)
{
    int rep;
    
    rep = Send_msg(server_socket, requete, strlen(requete));
    if (rep <= 0) {
        return rep;
    }
}

/* Always free the returned reply */
int OVESP_RECEIVE(char **reply, int server_socket)
{
    int rep;
    Message *msg;

    msg = NULL;
    *reply = NULL;
    
    rep = Receive_msg(server_socket, &msg);
    
    if (rep > 0) {
        *reply = (char *)malloc(sizeof(char)*msg->data_size);
        if (reply == NULL) {
            destroyMessage(msg);
            return NULL;
        }

        memcpy(*reply, msg->data, msg->data_size);
    }
    
    destroyMessage(msg);
    return rep;
        
}
int OVESP(char *requete, char *reponse, int socket)
{
    char *deli = strtok(requete, "#");

    if (strcmp(deli, "LOGIN") == 0)
    {
        /*Code du login*/
    }
}
int OVESP_Login(const char *user, const char *password, int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "LOGIN#%s#%s", user, password);
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Consult(int idArticle, int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "CONSULT#%d", idArticle);
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Achat(int idArticle, int quantite, int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "ACHAT#%d#%d", idArticle, quantite);
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Caddie(int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "CADDIE#");
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Cancel(int idArticle, int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "CANCEL#%d", idArticle);
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Cancel_All(int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "CANCELALL#");
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Confirmer(int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "CONFIRMER#");
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
int OVESP_Logout(int server_socket)
{
    char requete[100];
    int error_handling;

    sprintf(requete, "LOGOUT#");
    if ((error_handling = OVESP_SEND(requete, server_socket)) == 0 || error_handling == -1)
        return error_handling;
}
