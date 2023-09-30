#ifndef __SQL_REQUESTS_H__
#define __SQL_REQUESTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include <pthread.h>

/* SQL Settings*/
#define SQL_HOST "localhost"
#define SQL_USER "Student"
#define SQL_PASS "PassStudent1_"
#define SQL_DB "PourStudent"

struct Sql_request{
    ssize_t rows;
    ssize_t columns_per_row;
    char ***array_request;
};


int sql_requests_init(void);
struct Sql_request* sql_get_all_users(void);
void destroy_sql_request(struct Sql_request *request);

#endif 
