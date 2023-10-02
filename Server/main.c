#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "sockets.h"
#include "server.h"

int main()
{
    //int server_socket;
    //server_socket =
    Server_init();
    int return_val;

    // /int client_Socket;
    /*
    while(1) {
        client_Socket = Accept_connexion(server_socket);
        add_client(client_Socket);
        
    }
    */

    return_val = client_check_creds("Liwinux", "tes");
    if (return_val == -1) {
        printf("Database error !\n");
    }
    else if (return_val == 0) {
        printf("Successfully connecetd 1\n");
    }
    else if (return_val == 1) {
        printf("User doesnt exist in the database 1\n");
    }
    else if (return_val == 2) {
        printf("Bad password for the user !\n");
    }

    return 0;
}