#include "mysql_con.h"
mysql_con *meta_conn;
/*
    Function: ng_create_connect
    Description: create new mysql connection
    Return: NULL, create failed
                    nonull, create success */
bool
mysql_con::ng_create_connect(char *host, char *user, char *pwd, char * db, unsigned int port)
{
    /*Begin: add by wangfeng in 2013.10.28*/
    mysql_init(&connect);
    /*end: add by wangfeng in 2013.10.28*/
    if (!mysql_real_connect(&connect, host, user, pwd, db, port, NULL, 0))
    {
        printf("Eror connecting to database: %s, error code:%d, "
                " (host=%s, user=%s, "
                "db=%s, port=%u)",
                mysql_error(&connect), mysql_errno(&connect), host, user,
                db, port);printf("----\n");
        if (connect.free_me)
        {
            mysql_close(&connect);
        }
        return false;
    }
    connect.reconnect = 1;
    return true;
}

/* 
Function: ng_query.
Description: for SELECT, SHOW, SET.
Return: NONULL, query success. 
                NULL, query failed. */
void*
mysql_con::ng_query(char *sql)
{
    MYSQL_RES *res;
    int flag = 0;

    flag = mysql_real_query(&connect, sql, (unsigned long) strlen(sql));
    if (flag)
    {
        printf("Error code:%d, %s\n", mysql_errno(&connect), mysql_error(&connect));
        return NULL;
    }
   
    res = mysql_store_result(&connect);
    if (res == NULL)
    {
        printf("Error code:%d, %s\n", mysql_errno(&connect), mysql_error(&connect));
        return NULL;
    }
    return (void *) res;
}

/* 
Function: ng_update.
Description: for UPDATE, DELETE, INSERT, etc.
Return: 0, query success. 
                1, query failed. */
bool
mysql_con::ng_update(char *sql)
{
    int flag = 0;
    flag = mysql_real_query(&connect, sql, (unsigned long) strlen(sql));
    if (flag)
    {
        printf("Error code:%d, %s\n", mysql_errno(&connect), mysql_error(&connect));
        return 1;
    }
    return 0;
}

void
mysql_con::ng_sql_close()
{
    mysql_close(&connect);
    return;
}

