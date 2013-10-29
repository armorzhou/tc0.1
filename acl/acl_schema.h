/* 
 * File:   ACL_SCHEMA.h
 * Author: tangchao
 *
 * Created on 2013年9月6日, 下午7:23
 */

#ifndef SQL_RESOUCE_H
#define	SQL_RESOUCE_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>
#include <string.h>
#include <mysql.h>
#include "acl_db.h"

using namespace std;

class ACL_SCHEMA : public ACL_ACCESS {
public:

    ACL_SCHEMA(bool is_super, string user, string hostname, string pass, long long privileges) :
    ACL_ACCESS(privileges), super(is_super), username(user),
    host(hostname), password(pass) {
        memset(salt, 0, SCRAMBLE_LENGTH + 1);
        this->tc_get_salt_from_password(salt, pass.size() ? pass.c_str() : "");
    }

    ACL_SCHEMA(const ACL_SCHEMA& orig) :
    ACL_ACCESS(orig), super(orig.super), username(orig.username),
    host(orig.host), password(orig.password) {
        memset(salt, 0, SCRAMBLE_LENGTH + 1);
        memcpy(salt, orig.salt, SCRAMBLE_LENGTH + 1);
    }

    virtual ~ACL_SCHEMA() {
    }

    void set_username(string username);
    string get_username();
    void set_super(bool flag);
    bool get_super();
    string get_password();
    void set_host(string host);
    string get_host();

    bool add_one_db(string db_name, bool level, long long privilege);
    void delete_one_db(string *db_name);

    ACL_DB* find_db(string db_name);
    ACL_DB* find_db(char* db_name);
    bool check_db(string db_name, long long priv);
    bool check_table(string db_name, string table_name, long long priv);

    void tc_get_salt_from_password(unsigned char *hash_stage2, const char *password);
    unsigned char *get_salt();
    void print();
private:
    bool super;
    string username;
    string host; /* ip address */
    string password;
    unsigned char salt[SCRAMBLE_LENGTH + 1]; // scrambled password in binary form
    /* key:database name, value:database */
    map<string, ACL_DB> schema;
};

#endif	/* SQL_RESOUCE_H */
