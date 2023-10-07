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

#define SQL_DB_ERROR -1

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
 * @return SQL_DB_ERROR : if connection to the database has failed. 
 */
int sql_requests_init(void);


/**
 * @brief Create a new user inside the connected database.
 * 
 * @param username The username to create.
 * @param password The password to addign to this newly created user.
 * @return 0 if the user has successfully been added to the database.
 * @return 1 if the user already exists inside the database.
 * @return SQL_DB_ERROR if the user could't be created.
 */
int sql_create_new_user(char *username, char *password);

/**
 * @brief Checks the creds for a given username and password inside the database.
 * 
 * @param username The username to check in the database.
 * @param password The password to check in the database.
 * @return 0 : The password and the username matches.
 * @return 1 : The username doesn't exists in the database.
 * @return 2 : The password doesn't match the username.
 * @return SQL_DB_ERROR : An error in the database occured. 
 */
int sql_client_check_creds(char *username, char *password);

/**
 * @brief 
 * 
 * @param idArticle The article fields whose ID is @param idArticle.
 * @param result a pointer that will be allocated and contains the articles's fields whose ID is @param idArticle.
 * @return 0 : The article could be return and the pointer has been allocated with the results.
 * @return 1 : The article is not in the database.
 * @return SQL_DB_ERROR : An error in the database or malloc error occured. 
 */
int sql_consult(char *idArticle, Sql_result **result);

/**
 * @brief deallocate memory for the after a sql request.
 * 
 * @param request the request to destroy from memory.
 */
void destroy_sql_result(Sql_result *request);

#endif 
