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
    int error_check = 0;
    server_socket = Server_connect("localhost", 4444);
    error_check = OVESP_Login("liwinux", "test", '0', server_socket);
    switch(error_check) {
        case 0 :
            printf("Connection successfull !\n");
            break;
        case 1 : 
            printf("username doesn't exist !\n");
            break;
        
        case 2: 
            printf("Password incorrect !\n");
            break;
        case 3:
            printf("Database error !\n");
            break;
        
        case 4:
            printf("Username already taken !\n");
            break;
        default:
            break;
        
    }
    printf("OVESP_Login : %d\n", error_check);

}