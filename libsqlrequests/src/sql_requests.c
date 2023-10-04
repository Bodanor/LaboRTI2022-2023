#include "sql_requests.h"

/**
 * @file sql_requests.c
 * @author Liwinux & Tinmar1010
 * @brief Main functions definitions used to make requests to the database.
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */


/**
 * @brief Send a sql request and return the results.
 * 
 * @param request_str the request to send.
 * @return Sql_result* : a pointer containing the result from the request.
 * @return NULL : a malloc error occured or a database error occured.
 */
static Sql_result* sql_get_result(char *request_str);

/* SQL main connection. Only one connexion*/
static MYSQL *connexion;

/* Mutex used when a request is sent to the database */
static pthread_mutex_t mutexDB;


Sql_result* sql_get_result(char *request_str)
{
    MYSQL_RES *resultat;
    MYSQL_ROW ligne;
    int i;
    int x;
    Sql_result *request;

    if (mysql_query(connexion, request_str) == 0) {
        
        if ((resultat = mysql_store_result(connexion)) == NULL) {
            perror("Request error");
            return NULL;
        }
        else{
            /* Allocate memory for a request result */
            request = (Sql_result *)malloc(sizeof(Sql_result));
            if (request == NULL) {
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
                    return NULL;
                }
                /* Loop through all the users */
                for (i = 0; i < request->rows; i++) {
                    /* Allocate pointer per column for the current user index */
                    request->array_request[i] = (char **)malloc(sizeof(char*)*request->columns_per_row);
                    if (request->array_request[i] == NULL) {
                        return NULL;
                    }
                    
                    if ((ligne = mysql_fetch_row(resultat)) != NULL)
                    {
                        /* Loop through all the columns per row */
                        for (x = 0; x < request->columns_per_row; x++) {
                            /* Allocate a final pointer which will hold the column value */
                            request->array_request[i][x] = (char*)malloc(sizeof(char)*strlen(ligne[x]) + 1);
                            if (request->array_request[i][x] == NULL) {
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
        return NULL;
    }

    mysql_free_result(resultat); /* Free the result */
    return request;
}



int sql_requests_init(void)
{
    pthread_mutex_init(&mutexDB, NULL);

    connexion = mysql_init(NULL);
    /* Can we reach the database ? */
    if (mysql_real_connect(connexion, SQL_HOST, SQL_USER, SQL_PASS, SQL_DB, 0, 0, 0) == NULL)
    {
        fprintf(stderr, "(SERVEUR) Erreur de connexion à la base de données...\n");
        return -1;
    }
    return 0;
}

Sql_result* sql_get_all_users(void)
{
    char *request_str = "select login from clients"; /* Request to send */
    Sql_result* results;

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/

    results = sql_get_result(request_str);
    if (results == NULL) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return NULL;
    }
    
    /* Release the mutex and return the request result */

    pthread_mutex_unlock(&mutexDB);
    return results;
    
}

void destroy_sql_result(Sql_result *request)
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
int sql_add_client(char *username, char *password)
{
    char request_str[200];

    sprintf(request_str, "insert into clients values (0, \"%s\", \"%s\");", username, password); /* Request to send */

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/
    if (mysql_query(connexion, request_str) != 0) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return -1;
    }
    
    /* Release the mutex and return 0 */
    pthread_mutex_unlock(&mutexDB);
    return 0;


}
Sql_result* sql_get_user_password(char *username)
{
    char request_str[200];
    Sql_result* results;

    sprintf(request_str, "select password from clients where login = \"%s\";", username); /* Request to send */

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/

    results = sql_get_result(request_str);
    if (results == NULL) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return NULL;
    }
    
    /* Release the mutex and return the request result */
    pthread_mutex_unlock(&mutexDB);
    return results;

}
Sql_result* sql_get_article(char *idArticle)
{
    char request[200];
    Sql_result *results;
    /*Il faut créer la table article */
    sprintf(request, "select * from article where id = %s;", idArticle);

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/

    results = sql_get_result(request);
    if (results == NULL) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return NULL;
    }
    
    /* Release the mutex and return the request result */
    pthread_mutex_unlock(&mutexDB);
    return results;
}
Sql_result* sql_get_all_articles(void)
{
    char *request_str = "select * from articles"; /* Request to send */
    Sql_result* results;

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/

    results = sql_get_result(request_str);
    if (results == NULL) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return NULL;
    }
    
    /* Release the mutex and return the request result */

    pthread_mutex_unlock(&mutexDB);
    return results;
    
}