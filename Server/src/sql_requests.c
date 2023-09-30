#include "sql_requests.h"

/* SQL main connection*/
static MYSQL *connexion;

static pthread_mutex_t mutexDB;


int sql_requests_init(void)
{
    pthread_mutex_init(&mutexDB, NULL);

    connexion = mysql_init(NULL);
    if (mysql_real_connect(connexion, SQL_HOST, SQL_USER, SQL_PASS, SQL_DB, 0, 0, 0) == NULL)
    {
        fprintf(stderr, "(SERVEUR) Erreur de connexion à la base de données...\n");
        return -1;
    }
    return 0;
}

struct Sql_request* sql_get_all_users(void)
{
    char *request_str = "select username from clients";
    MYSQL_RES *resultat;
    MYSQL_ROW ligne;
    int i;
    int x;
    struct Sql_request *request;

    pthread_mutex_lock(&mutexDB);

    if (mysql_query(connexion, request_str) == 0) {
        
        if ((resultat = mysql_store_result(connexion)) == NULL) {
            perror("Request error");
            pthread_mutex_unlock(&mutexDB);
            return NULL;
        }
        else{
            /* Allocate memory for a request result */
            request = (struct Sql_request *)malloc(sizeof(struct Sql_request));
            if (request == NULL) {
                pthread_mutex_unlock(&mutexDB);
                return NULL;
            }
            else {
                /* Get the number of users inside the database */
                request->rows = mysql_num_rows(resultat);
                /* Get the number of rows columns per row */
                request->columns_per_row = mysql_num_fields(resultat);
                /* Allocate enough pointers per users */
                request->array_request = (char***)malloc(sizeof(char**)*request->rows);
                if (request->array_request == NULL) {
                    pthread_mutex_unlock(&mutexDB);
                    return NULL;
                }
                /* Loop through all the users */
                for (i = 0; i < request->rows; i++) {
                    /* Allocate pointer per column for the current user index */
                    request->array_request[i] = (char **)malloc(sizeof(char*)*request->columns_per_row);
                    if (request->array_request[i] == NULL) {
                        pthread_mutex_unlock(&mutexDB);
                        return NULL;
                    }
                    
                    if ((ligne = mysql_fetch_row(resultat)) != NULL)
                    {
                        /* Loop through all the columns per row */
                        for (x = 0; x < request->columns_per_row; x++) {
                            /* Allocate a final pointer which will hold the column value */
                            request->array_request[i][x] = (char*)malloc(sizeof(char)*strlen(ligne[x]));
                            if (request->array_request[i][x] == NULL) {
                                pthread_mutex_unlock(&mutexDB);
                                return NULL;
                            }
                            /* Copy the column to the current user index + column index */
                            strcpy(request->array_request[i][x], ligne[x]);

                            /* Result looks like this inside memory for this SQL result :
                            ** username:       password:
                            ** FOO1            DUMMY
                            ** FOO2            DUMMY2
                            **
                            **
                            ** ptr*** (row pointer AKA : ROW 0)    --> ptr** (column pointer AKA : FOO1)-->    ptr* (column string AKA : DUMMY)
                            ** ptr*** (row pointer AKA : ROW 1)    --> ptr** (column pointer AKA : FOO2)-->    ptr* (column string AKA : DUMMY2)
                            */
                        }
                        
                    }
                    
                }

            }

        }
    }
    else {
        pthread_mutex_unlock(&mutexDB);
        return NULL;
    }
    mysql_free_result(resultat);
    pthread_mutex_unlock(&mutexDB);
    return request;
}

void destroy_sql_request(struct Sql_request *request)
{
    int i;
    int x;

    for (i = 0; i < request->rows; i++)
        for (x = 0; x < request->columns_per_row; x++) {
            free(request->array_request[i][x]);
        }
    free(request->array_request);
    free(request);

}