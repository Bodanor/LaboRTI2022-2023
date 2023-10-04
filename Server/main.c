#include <stdio.h>
#include <pthread.h>
#include <getopt.h>
#include <stdlib.h>

#include "OVESP.h"
#include "server.h"

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
    char *config_file;
    int i;
    int server_socket;
    int client_socket;
    int error_check;

    config_file = NULL;
    port = threads = -1;

    const char *optstring = "hp:t:";
    const struct option lopts[] = {
        {"help", no_argument, NULL, 'h'}, /* Print the help menu */
        {"port", required_argument, NULL, 'p'}, /* Required the port number to listen to */
        {"threads", required_argument, NULL, 't'}, /* Number of threads to create */
        {"config", required_argument, NULL, 'c'},
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
                if (check_is_number(optarg) == 1) {
                    printf("Bad argument passed for the threads numbers. Argument is not a number\n");
                    break;
                }
                threads = atoi(optarg);
                break;
            case 'p' :
                if (check_is_number(optarg) == 1) {
                    printf("Bad argument passed for the port numbers. Argument is not a port\n");
                    break;
                }
                port = atoi(optarg);
                break;
            case 'c':
                config_file = (char*)malloc(sizeof(char)*strlen(optarg) + 1);
                if (config_file == NULL) {
                    printf("Malloc error !\n");
                    return -1;
                }
                strcpy(config_file, optarg);
                break;
        }
        index = -1;
        
    }

    if (optind < argc)
    {
      while (optind < argc)
        printf ("Non option \"%s\". Ignoring it.\n", argv[optind++]);
    }
    
    server_socket = Server_init(port, threads, config_file);


    while (1) {
        client_socket = Accept_connexion(server_socket);
        error_check = OVESP_server(client_socket);
        if (error_check == -1) {
            printf("Client deconnecte \n");
        }
        else if (error_check == -2) {
            printf("Corrupted data\n");
        }
        else if (error_check == -3) {
            printf("I/O error \n");
        }
        else if (error_check == -4) {
            printf("Internal error\n");
        }
    
    }

    return 0; 
}