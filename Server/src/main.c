#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "sockets.h"
#include "server.h"
#include "OVESP.h"

int main()
{
    int server_socket;
    int client_socket;
    int error_check = 0;
    Message *msg;

    msg = NULL;
    server_socket = Server_init();

    while (1) {
        client_socket = Accept_connexion(server_socket);
        Receive_msg(client_socket, &msg);
        error_check = OVESP_server((char*)msg->data, client_socket);
        printf("OVESP_SERVER : %d\n", error_check);
        
    }

    return 0;
}