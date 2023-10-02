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
static int OVESP_RETRIEVE_STATUS(char **reply, int server_socket);

int OVESP_RETRIEVE_STATUS(char **reply, int server_socket)
{
    int error;
    char *response;

    response = NULL;
    
    error = OVESP_RECEIVE(&response, server_socket);
    if(error <= 0)
    {
        return error; /* Deconnexion du serveur*/
    }
    
    strtok(response, "#"); /* Used to get the next parameter in the reponse */
    *reply = strtok(response, "#");
    
    return (strlen(*reply));
}

int OVESP_SEND(char *requete, int server_socket)
{
    int rep;
    
    rep = Send_msg(server_socket, (uint8_t*)requete, strlen(requete));
    
    return rep;
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
            return -2;
        }

        memcpy(*reply, msg->data, msg->data_size);
    }
    
    destroyMessage(msg);
    return rep;
        
}
int OVESP(char *requete, char *reponse, int socket)
{
    char *champs1 = strtok(requete, "#");
    char *champs2;
    char *champs3;
    char *champs4;
    int rep, rep1;

    if (strcmp(champs1, "LOGIN") == 0)
    {
        champs2 = strtok(requete, "#");
        champs3 = strtok(requete, "#");
        champs4 = strtok(requete, "#");

        if(strcmp(champs4, "O") == 0)
        {
            if((rep = client_check_creds(champs2, champs3)) == 0)
            {

                rep1 = OVESP_SEND("LOGIN#OK#", socket);

            }
            else if (rep == 1)
            {
                rep1 = OVESP_SEND("LOGIN#KO_Baduser#", socket);
            }
            else if (rep == 2)
            {
                rep1 = OVESP_SEND("LOGIN#KO_Badpassword#", socket);
            }
            else 
            {
                rep1 = OVESP_SEND("LOGIN#KO_DB_FAIL#", socket);
            }
        }
        else
        {
            if((rep = create_new_user(champs2, champs3)) == 0)
            {

                rep1 = OVESP_SEND("LOGIN#OK#", socket);

            }
            if (rep == 1)
            {
                rep1 = OVESP_SEND("LOGIN#KO_Userexists#", socket);
            }
            else if(rep == -1)
            {
                rep1 = OVESP_SEND("LOGIN#KO_DB_FAIL#", socket);
            }
        }

        return rep1;
    }
}
/**
 * @brief 
 * 
 * @param user 
 * @param password 
 * @param new_user_flag 
 * @param server_socket 
 * @return 0 : Good
 * @return 1 : bad user
 * @return 2 : bad password
 * @return 3 : User already present
 * @return 4 : Server DB fail
 * @return -1 : Server RIP
 * @return -2 : Network error
 * @return -3 : Network error
 */
int OVESP_Login(const char *user, const char *password, const char new_user_flag, int server_socket)
{
    char requete[100];
    int error;
    char *response;

    sprintf(requete, "LOGIN#%s#%s#%c#", user, password, new_user_flag);
    error = OVESP_SEND(requete, server_socket);
    if (error < 0)
        return error;
    
    error = OVESP_RETRIEVE_STATUS(&response, server_socket);
    if (error < 0)
        return error;
    
    if(strcmp(response, LOGIN_OK) == 0)
        return 0;
    else if (strcmp(response, LOGIN_BAD_USER) == 0 )
        return 1;
    else if (strcmp(response, LOGIN_BAD_PASSWORD) == 0 )
        return 2;
    else if (strcmp(response, LOGIN_ALREADY_EXISTS) == 0)
        return 3;
    else if (strcmp(response, LOGIN_DB_FAIL) == 0)
        return 4;
    /* else jsp*/
    
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
