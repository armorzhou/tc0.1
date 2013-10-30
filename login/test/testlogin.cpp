/* 
 * File:   testlogin.cpp
 * Author: tangchao
 *
 * Created on 2013年10月25日, 下午3:40
 */

#include <cstdlib>
#include <time.h>
#include <mysql.h>
#include "../../acl/sql_acl.h"
#include "../../connector/mysql_con.h"
#include "../../listener/listener.h"
#include "../login55.h"

using namespace std;

/*
 * 
 */
struct rand_struct password_rand;

time_t my_time(int flags)
{
    time_t t;
    /* The following loop is here beacuse time() may fail on some systems */
    while ((t = time(0)) == (time_t) - 1)
    {
        if (flags & 16)
            printf("Wrning: time() call failed.\n");
    }
    return t;
}

void set_random()
{
    time_t server_start_time = my_time(0);
    randominit(&password_rand, (unsigned long) server_start_time,
            (unsigned long) server_start_time / 2);
}

int main(int argc, char** argv)
{
    int socket;
    unsigned int port = 4455;

    set_random();

    meta_conn = new mysql_con();
    meta_conn->ng_create_connect("172.17.4.50", "dba", "yvhkfhvk", "joy_dbcluster_ii", 3333);

    SQL_ACL::get_instance(false)->init();
    SQL_ACL::get_instance(false)->print();

    LISTENER *listener = new LISTENER();
    SERVER_CONNECTION_CNF *scc = new SERVER_CONNECTION_CNF();
    PROTOCOL *pl = new PROTOCOL_10;
    LOGIN *lg = new LOGIN55();

    listener->init(&socket, NULL, &port);

    int sd = accept(socket, NULL, NULL);
    pl->init(sd, 0400);
    lg->init(scc, pl);
    if (!lg->check_login())
    {
        printf("login error\n");
    }
    else
    {
        printf("login ok\n");
    }
    return 0;
}
