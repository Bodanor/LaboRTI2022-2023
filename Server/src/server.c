#include "server.h"

/* static void HandlerSIGINT(int s); */
static void load_config_file(int *listening_port, int *max_threads);


/* Mutex related */
static pthread_mutex_t mutexAcceptedSockets;
static pthread_cond_t condAcceptedSockets;

/* Server variables */
static int AcceptedSockets[MAX_QUEUE];
//static int accepted_read_index = 0;
static int accepted_write_index = 0;
static int max_threads = 0;
static int PORT_ACHAT = 0;

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

    pthread_mutex_init(&mutexAcceptedSockets, NULL);
    pthread_cond_init(&condAcceptedSockets, NULL);

    load_config_file(&PORT_ACHAT, &max_threads);

    server_socket = Create_server(PORT_ACHAT);

    for(i = 0; i < MAX_QUEUE; i++)
        AcceptedSockets[i] = -1;

    printf("Creating threads pool of size (%d)\n", max_threads);
    
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
    pthread_mutex_lock(&mutexAcceptedSockets);
    AcceptedSockets[accepted_write_index] = client_socket;
    if (accepted_write_index == MAX_QUEUE)
            accepted_write_index = 0;
#ifdef DEBUG
    printf("[DEBUG] Current connected client sockets : \n\n");
    for (int i = 0; i < max_threads; i++)
        printf("[Array : %d] --> %d\n", i, AcceptedSockets[i]);
#endif
    
    pthread_mutex_unlock(&mutexAcceptedSockets);
    pthread_cond_signal(&condAcceptedSockets);
}

int username_already_exists(char *username)
{
    int i;
    int x;
    struct Sql_request *results;

    results = sql_get_all_users();
    if (results == NULL)
        return -1;
    
    else {
        for(i = 0; i < results->rows; i++) {
            for (x = 0; x < results->columns_per_row; x++) {
                if (strcmp(results->array_request[i][x], username) == 0)
                    return 0;
            }
        }
    }
    destroy_sql_request(results);

    return 1;

}