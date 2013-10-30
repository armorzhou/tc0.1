/* 
 * File:   SQL_ACL.h
 * Author: tangchao
 *
 * Created on 2013年8月14日, 下午1:24
 */

#ifndef SQL_ACL_H
#define	SQL_ACL_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>
#include <pthread.h>
#include "schema.h"
#include "acl_schema.h"
#include "acl_convert.h"

using namespace std;

class SQL_ACL : public ACL_CONVERT
{
private:
    bool skip_grant;
    pthread_mutex_t sql_acl_lock;
    map<SCHEMA, ACL_SCHEMA> acl_user;
    static SQL_ACL *acl;

    SQL_ACL(bool skip_grant) : skip_grant(skip_grant)
    {
        pthread_mutex_init(&sql_acl_lock, NULL);
    }
public:
    static SQL_ACL* get_instance(bool flag)
    {
        if (acl == NULL)
            acl = new SQL_ACL(flag);
        return acl;
    }
    bool reset();
    void sql_acl_clean();
    long long get_acl_size();
    void set_skip_grant(bool flag);
    bool get_skip_grant();
    SCHEMA get_user_key(string user, string host);
    SCHEMA get_user_key(char* user, char* host);
    bool init();
    bool acl_init_user();
    bool acl_init_db();
    bool acl_init_table();
    bool create_one_schema(SCHEMA schema, string password, bool flag);
    bool add_one_user(SCHEMA schema, string password, bool flag, long long priv);
    bool put_db_privilege_into_schema(SCHEMA schema, string db_name,
            bool flag, long long priv);
    bool put_table_privilege_into_schema(char *user, char*host, char *db_name,
            char *table_name, char *privilege);
    ACL_SCHEMA* find_user(SCHEMA schema);
    ACL_SCHEMA* find_user(SCHEMA *schema);
    bool acl_check_host(char* ip);
    void print();

    char* get_user_privilege();
    char* get_db_privilege();
    char *get_table_privilege();
};
#endif	/* SQL_ACL_H */
