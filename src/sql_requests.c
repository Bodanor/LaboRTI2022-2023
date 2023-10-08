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

/**
 * @brief Function to add a new user to the database.
 * 
 * @param username username for the new client.
 * @param password password fot the new client.
 * @return 0 : If the user has successfully been added to the database.
 * @return SQL_DB_ERROR: If the database returned an error.
 */
int sql_add_client(char *username, char *password);

/**
 * @brief return all the users inside the database.
 * 
 * @return Sql_result* : containing all the users.
 * @return NULL : a database error occured. 
 */
static Sql_result* sql_get_all_users(void);

/**
 * @brief return all the articles inside the database.
 * 
 * @return Sql_result* : containing all the articles.
 * @return NULL : a database error occured. 
 */
static Sql_result* sql_get_all_articles(void);

/**
 * @brief return the password for the given username.
 * 
 * @param username : The username password to return.
 * @return Sql_result* : containing the password.
 * @return NULL : database error or malloc error.
 */
static Sql_result* sql_get_user_password(char *username);


/**
 * @brief Checks if a username already exists in the database
 * 
 * @param username The username to check against the database.
 * @return 0 : The username is present in the database.
 * @return 1 : The username is not in the database.
 * @return SQL_DB_ERROR : An error in the database occured. 
 */
static int sql_username_already_exists(char *username);

/**
 * @brief Checks if an article is present in the database.
 * 
 * @param idArticle The id of the article to check.
 * @param result Pointer that stores the sql_query result.
 * @return 0 : The article is in the database.
 * @return 1 : The article is not the database
 * @return SQL_DB_ERROR : An error in the database occured. 
 */
static int sql_articles_already_exists(char *username);

/**
 * @brief Return article for the given id.
 * 
 * @param idArticle The Id of the article to return.
 * @return Sql_result* : Containing the article fields of the given article ID.
 * @return NULL : database error or malloc error.
 */
static Sql_result* sql_get_article(char *idArticle);
static Sql_result* sql_article_get_stock(char *idArticle);
int sql_achat_possible(char* idArticle, char* quantity);
static Sql_result* sql_achat_article(char *idArticle, char *quantity);


/* SQL main connection. Only one connexion*/
static MYSQL *connexion;
/* Mutex used when a request is sent to the database */
static pthread_mutex_t mutexDB;



int sql_requests_init(void)
{
    pthread_mutex_init(&mutexDB, NULL);

    connexion = mysql_init(NULL);
    /* Can we reach the database ? */
    if (mysql_real_connect(connexion, SQL_HOST, SQL_USER, SQL_PASS, SQL_DB, 0, 0, 0) == NULL)
    {
        fprintf(stderr, "(SERVEUR) Erreur de connexion à la base de données...\n");
        return SQL_DB_ERROR;
    }
    return 0;
}

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

Sql_result* sql_get_all_articles(void)
{
    char *request_str = "select id from articles"; /* Request to send */
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
    sprintf(request, "select * from articles where id = %s;", idArticle);

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

static Sql_result* sql_article_get_stock(char *idArticle)
{
    char request[200];
    Sql_result *results;
    
    sprintf(request, "select stock from articles where id = %s;", idArticle);

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
int sql_username_already_exists(char *username)
{
    int i;
    Sql_result *results;

    /* Get a list of all the users */
    results = sql_get_all_users();
    if (results == NULL)
        return SQL_DB_ERROR;
    
    else {
        /* Iterate all the rows (row numbers) + columns (usernames) and compare*/
        for(i = 0; i < results->rows; i++) {
            /* If the username is found, return 1*/
            if (strcmp(results->array_request[i][0], username) == 0) {
                sql_destroy_result(results); /* Dont forget to free the sql_result */
                return 0;
            }
            
        }
    }
    sql_destroy_result(results); /* Dont forget to free the sql_result */

    return 1;

}

int sql_articles_already_exists(char *idArticle)
{
    int i;

    Sql_result *results;

    /* Get a list of all the users */
    results = sql_get_all_articles();
    if (results == NULL)
        return SQL_DB_ERROR;
    
    else {
        /* Iterate all the rows (row numbers) + columns (usernames) and compare*/
        for(i = 0; i < results->rows; i++) {
            /* If the article ID is found, return 0*/
            if (strcmp(results->array_request[i][0], idArticle) == 0) {
                sql_destroy_result(results); /* Dont forget to free the sql_result */
                return 0;
            }
            
        }
    }
    sql_destroy_result(results);

    return 1;
}

int sql_add_client(char *username, char *password)
{
    char request_str[200];

    sprintf(request_str, "insert into clients values (0, \"%s\", \"%s\");", username, password); /* Request to send */

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/
    if (mysql_query(connexion, request_str) != 0) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return SQL_DB_ERROR;
    }
    
    /* Release the mutex and return 0 */
    pthread_mutex_unlock(&mutexDB);
    return 0;
}

int sql_create_new_user(char *username, char *password)
{
    int error_check;

    /* If error or user already exist, return 1*/
    if ((error_check = sql_username_already_exists(username)) == 0) {
        return 1;
    }
    
    else {
        if (sql_add_client(username, password) != 0 ) {
            /* Could not create the user*/
            return SQL_DB_ERROR;
        }

    }
    return 0; /* User successfully created */
}

int sql_client_check_creds(char *username, char *password)
{
    int error_check;
    int i;
    int x;
    Sql_result *results;
    
    results = NULL;

    if ((error_check = sql_username_already_exists(username)) != 0) {
        /* Database error */
        return error_check;
    }
    
    /* User exist in the database, whe have to check the password */
    results = sql_get_user_password(username);
    if (results == NULL)
        return SQL_DB_ERROR;

    else {
        for(i = 0; i < results->rows; i++) {
            for (x = 0; x < results->columns_per_row; x++) {
                /* If the password matches the user return 0 */
                if (strcmp(results->array_request[i][x], password) == 0) {
                    sql_destroy_result(results); /* Dont forget to free the sql_result */
                    return 0;
                }
            }
        }
    }
    sql_destroy_result(results); /* Dont forget to free the sql_result */
    return 2;
    
}

int sql_consult(char *idArticle, Sql_result **result)
{
    int error_check;
    
    *result = NULL;

    if ((error_check = sql_articles_already_exists(idArticle)) != 0) {
        /* Database error */
        return error_check;
    }
    *result  = sql_get_article(idArticle);
    if (*result == NULL)
        return SQL_DB_ERROR;

    return 0;
}

/**
 * @brief Function to check if an article can be bought by it's quantity.
 * 
 * @param idArticle The article ID to buy.
 * @param quantity The quantity to buy. 
 * @return 0 : The article cannot be bought
 * @return 1 : The article can be bought.
 * @return SQL_DB_ERROR : A database error occured.
 */
int sql_achat_possible(char *idArticle, char *quantity)
{
    Sql_result*results;
    int possible;

    results = sql_article_get_stock(idArticle);
    if (results == NULL)
        return SQL_DB_ERROR;

    if (atoi(quantity) > atoi(results->array_request[0][0]))
        possible = 0;
    else
        possible = 1;

    sql_destroy_result(results);
    return possible;
}

/**
 * @brief Buy an item from the database and update it.
 * 
 * @param idArticle The article Id to buy.
 * @param quantity The quantity to buy.
 * @return Sql_result A pointer to the updated bought article.
 * @return NULL : If a database or malloc error occured.
 */
Sql_result* sql_achat_article(char *idArticle, char *quantity)
{
    char request_str[200];
    Sql_result*results;

    sprintf(request_str, "update articles set stock =stock-%d where id=%d", atoi(quantity), atoi(idArticle));

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/
    if (mysql_query(connexion, request_str) != 0) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return NULL;
    }
    
    /* Release the mutex */
    pthread_mutex_unlock(&mutexDB);

    results = sql_get_article(idArticle);
    results->array_request[0][3] = (char*) realloc(results->array_request[0][3], strlen(quantity) + 1);
    strcpy(results->array_request[0][3], quantity); 
    if (results == NULL)
        return NULL;
    
    return results;
}

int sql_achat(char *idArticle, char *quantite, Sql_result **result)
{
    int error_check;

    *result = NULL;

    if ((error_check = sql_articles_already_exists(idArticle)) == 1 || error_check == SQL_DB_ERROR) {
        /* Database error */
        return error_check;
    }

    if ((error_check = sql_achat_possible(idArticle, quantite)) == 0)
        return 2;
    else if (error_check == SQL_DB_ERROR)
        return SQL_DB_ERROR;

    else {
        *result = sql_achat_article(idArticle, quantite);
        if (*result == NULL)
            return SQL_DB_ERROR;
    }

    return 0;
}
int sql_cancel(char *idArticle, char *quantity)
{
    int error_check;
    char request_str[200];

   

    if ((error_check = sql_articles_already_exists(idArticle)) == 1 || error_check == SQL_DB_ERROR) {
        /* Database error */
        return error_check;
    }
    sprintf(request_str, "update articles set stock =stock+%d where id=%d", atoi(quantity), atoi(idArticle));

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/
    if (mysql_query(connexion, request_str) != 0) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return -1;
    }
    
    /* Release the mutex */
    pthread_mutex_unlock(&mutexDB);


    return 0;

}
int sql_confirmer(Sql_result **result)
{
    int error_check;
    char request_str[200];


    sprintf(request_str, "");

    pthread_mutex_lock(&mutexDB); /* Lock the mutex*/
    if (mysql_query(connexion, request_str) != 0) {
        pthread_mutex_unlock(&mutexDB); /* Release the mutex if error */
        return -1;
    }
    
    /* Release the mutex */
    pthread_mutex_unlock(&mutexDB);


    return 0;

}
void sql_destroy_result(Sql_result *request)
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
