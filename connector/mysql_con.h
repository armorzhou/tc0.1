/*
 * File:   mysql_con.h
 * Author: tangchao
 *
 * Created on 2013年10月21日, 下午6:19
 */

#ifndef MYSQL_CON_H
#define	MYSQL_CON_H
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>

class mysql_con
{
public:
    virtual ~mysql_con() {}
     bool ng_create_connect(char *host, char *user, char *pwd,
            char * db, unsigned int port);
     void* ng_query(char *sql);
     bool ng_update(char *sql);
     void ng_sql_close();
private:
    MYSQL connect;
};
extern class mysql_con *meta_conn;
#endif	/* MYSQL_CON_H */

