/**
 * @file server.c
 * @author Liwinux & Tinmar1010
 * @brief Main functions definitions used to manage the server requests.
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "server.h"

struct server_t {
    pthread_mutex_t mutexAcceptedSockets;
    pthread_cond_t condAcceptedSockets;
    int AcceptedSockets[MAX_QUEUE];
    int accepted_write_index;
    int accepted_read_index;
    int max_threads;
    int PORT_ACHAT;

};

/* Only one instance of this struct is going to be created */
static Server_t server_struct;



/* static void HandlerSIGINT(int s); */

/**
 * @brief Load the configuration file if it exsists. Otherwise, default
 * settings are loaded.
 * 
 * @param listening_port server listening port according to the config
 * @param max_threads maximum threads the server can create.
 */
static void load_config_file(int *listening_port, int *max_threads);


int Server_init(void)
{
    struct sigaction A;
    int server_socket = 0;
    int i;
    /* pthread_t thread; */

    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
/*     A.sa_handler = HandlerSIGINT; */
    if (sigaction(SIGINT, &A, NULL) == -1) {
        perror("Sigation error");
        exit(1);
    }

    /* Sql requests to database initilization */
    if (sql_requests_init() == -1)
        return -1;

    pthread_mutex_init(&server_struct.mutexAcceptedSockets, NULL);
    pthread_cond_init(&server_struct.condAcceptedSockets, NULL);

    load_config_file(&server_struct.PORT_ACHAT, &server_struct.max_threads);

    server_socket = Create_server(server_struct.PORT_ACHAT);

    for(i = 0; i < MAX_QUEUE; i++)
        server_struct.AcceptedSockets[i] = -1;

    printf("Creating threads pool of size (%d)\n", server_struct.max_threads);
    
    /* for (i = 0; i < max_threads; i++)
        pthread_create(&thread, NULL, ClientFunction, NULL);
     */
    return server_socket;


}
/* Really bad code. Update it later...*/
void load_config_file(int *listening_port, int *max_threads)
{
    FILE *f_ptr;
    char bufr_key[100];
    char bufr_val[100];

    f_ptr = fopen("config.txt", "r");
    if (f_ptr == NULL) {
        *listening_port = DEFAULT_PORT_LISTENING;
        *max_threads = DEFAULT_MAX_THREADS;
    }
    else {
        if (fscanf(f_ptr, "%s = %s", bufr_key, bufr_val) != 2) {
            *listening_port = DEFAULT_PORT_LISTENING;
            *max_threads = DEFAULT_MAX_THREADS;
        }
        
        if (strcmp(bufr_key, "PORT_LISTEN") == 0) {
            *listening_port = atoi(bufr_val);
            if (*listening_port == 0)
                *listening_port = DEFAULT_PORT_LISTENING;
        }
        else {
            *listening_port = DEFAULT_PORT_LISTENING;
        }

        if (fscanf(f_ptr, "%s = %s", bufr_key, bufr_val) != 2) {
            *listening_port = DEFAULT_PORT_LISTENING;
            *max_threads = DEFAULT_MAX_THREADS;
        }
        
        if (strcmp(bufr_key, "MAX_THREADS") == 0) {
            *max_threads = atoi(bufr_val);
            if (*max_threads == 0)
                *max_threads = DEFAULT_MAX_THREADS;
        }
        else {
            *max_threads = DEFAULT_MAX_THREADS;
        }

    }
}

/* void HandlerSIGINT(int s)
{

}
 */


void add_client(int client_socket)
{
    pthread_mutex_lock(&server_struct.mutexAcceptedSockets);
    server_struct.AcceptedSockets[server_struct.accepted_write_index] = client_socket;
    if (server_struct.accepted_write_index == MAX_QUEUE)
            server_struct.accepted_write_index = 0;
#ifdef DEBUG
    printf("[DEBUG] Current connected client sockets : \n\n");
    int i;
    for (i = 0; i < server_struct.max_threads; i++)
        printf("[Array : %d] --> %d\n", i, server_struct.AcceptedSockets[i]);
#endif
    
    /* Release the mutex and wake one thread up */
    pthread_mutex_unlock(&server_struct.mutexAcceptedSockets);
    pthread_cond_signal(&server_struct.condAcceptedSockets);
}

int username_already_exists(char *username)
{
    int i;
    int x;
    Sql_result *results;

    /* Get a list of all the users */
    results = sql_get_all_users();
    if (results == NULL)
        return -1;
    
    else {
        /* Iterate all the rows (row numbers) + columns (usernames) and compare*/
        for(i = 0; i < results->rows; i++) {
            for (x = 0; x < results->columns_per_row; x++) {
                /* If the username is found, return 1*/
                if (strcmp(results->array_request[i][x], username) == 0) {
                    destroy_sql_result(results); /* Dont forget to free the sql_result */
                    return 1;
                }
            }
        }
    }
    destroy_sql_result(results); /* Dont forget to free the sql_result */

    return 0;

}
int create_new_user(char *username, char *password)
{
    /* If error or user already exist, return 1*/
    if (username_already_exists(username) <= 0) {
        return 1;
    }
    else {
        if (sql_add_client(username, password) == -1) {
            /* Could not create the user*/
            return -1;
        }

    }
    return 0; /* User successfully created */
}

int client_check_creds(char *username, char *password)
{
    int username_exist;
    int i;
    int x;
    Sql_result *results;
    
    if ((username_exist = username_already_exists(username)) == -1) {
        /* Database error */
        return -1;
    }
    else if (username_exist == 0) {
        /* User doesn't exist in the database, we should create a new one */
        return 1;
    }
    
    /* User exist in the database, whe have to check the password */
    results = sql_get_user_password(username);
    if (results == NULL)
        return -1;

    else {
        for(i = 0; i < results->rows; i++) {
            for (x = 0; x < results->columns_per_row; x++) {
                /* If the password matches the user return 0 */
                if (strcmp(results->array_request[i][x], password) == 0) {
                    destroy_sql_result(results); /* Dont forget to free the sql_result */
                    return 0;
                }
            }
        }
    }
    destroy_sql_result(results); /* Dont forget to free the sql_result */
    return 2;
    
}


/* Server destroy function ?*/