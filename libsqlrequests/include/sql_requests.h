#ifndef __SQL_REQUESTS_H__
#define __SQL_REQUESTS_H__

/**
 * @file sql_requests.h
 * @author Liwinux & Tinmar1010
 * @brief Main functions definitions used to make requests to the database.
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include <pthread.h>


#define SQL_HOST "localhost"
#define SQL_USER "Student"
#define SQL_PASS "PassStudent1_"
#define SQL_DB "PourStudent"

typedef struct Sql_t{
    ssize_t rows;
    ssize_t columns_per_row;
    char ***array_request;
} Sql_result;

/**
 * @brief Initiliaze the sql requests that is a mutex and check. 
 * if the connection can be done witht the database.
 * 
 * @return 0 : if the initilization is successfull.
 * @return -1 : if connection to the database has failed. 
 */
int sql_requests_init(void);

/**
 * @brief Function to add a new user to the database.
 * 
 * @param username username for the new client.
 * @param password password fot the new client.
 * @return 0 : If the user has successfully been added to the database.
 * @return -1: If the database returned an error.
 */
int sql_add_client(char *username, char *password);

/**
 * @brief return the password for the given username.
 * 
 * @param username : The username password to return.
 * @return Sql_result* : containing the password.
 * @return NULL : If the password could be retreived. Possible non-existing user
 * or database error.
 */
Sql_result* sql_get_user_password(char *username);

/**
 * @brief return all the users inside the database.
 * 
 * @return Sql_result* : containing all the users.
 * @return NULL : a database error occured. 
 */
Sql_result* sql_get_all_users(void);



/**
 * @brief deallocate memory for the after a sql request.
 * 
 * @param request the request to destroy from memory.
 */
void destroy_sql_result(Sql_result *request);

Sql_result* sql_get_article(char *idArticle);
Sql_result* sql_get_all_articles(void);

#endif 
