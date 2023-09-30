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
   return_val = username_already_exists("liwinux");
   printf("%d\n", return_val);
   return 0;
}