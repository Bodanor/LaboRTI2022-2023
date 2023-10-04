#include <stdio.h>
#include <pthread.h>
#include <getopt.h>
#include <stdlib.h>

#include "OVESP.h"

void show_help_menu (void);

void show_help_menu(void)
{
    printf("Usage : server [OPTIONS]\n");
    printf("Example : server -p [PORT NUMBER] -t [THREADS_NUMBERS]\n\n");
    printf("Options available :\n");
    printf("-p, --port\t PORT to use when the server is listening.\n");
    printf("-t, --threads\t THREADS Pool the server will create for the clients.\n");
    printf("-h, --help\t show this help menu.\n\n");
}

int main(int argc, char **argv)
{
    int port;
    int threads;
    int i;

    port = threads = 0;
    OVESP_Login("chris", "papa", 0, 0);

    const char *optstring = "hp:t:";
    const struct option lopts[] = {
        {"help", no_argument, NULL, 'h'}, /* Print the help menu */
        {"port", required_argument, NULL, 'p'}, /* Required the port number to listen to */
        {"threads", required_argument, NULL, 't'}, /* Number of threads to create */
        {NULL, no_argument, NULL, 0}, /* If no arguments, read the config file */
    };


    int val;
    int index;

    index = -1;

    while (EOF !=(val = getopt_long(argc, argv, optstring, lopts, &index))) {
        
        switch(val) {
            case 'h' :
                show_help_menu();
                break;
            
            case 't' :
            case 'p' :
                for (i = 0; i < (int)strlen(optarg); i++)
                if (isdigit(optarg[i]) == 0)
                {
                    printf("Bad argument for the port number. Argument is not a number\n");
                    return -1;
                }

            break;
            
        }
        index = -1;
        
    }

    /* int server_socket;
    int client_socket;
    Message *msg;

    msg = NULL;
    server_socket = Server_init();

    while (1) {
        client_socket = Accept_connexion(server_socket);
        Receive_msg(client_socket, &msg);
        OVESP_server((char*)msg->data, client_socket);
        
    }

    return 0; */
}