/* 
 * File:   server_connection_cnf.h
 * Author: tangchao
 *
 * Created on 2013年10月24日, 下午4:24
 */

#ifndef SERVER_CONNECTION_CNF_H
#define	SERVER_CONNECTION_CNF_H

#include <string.h>
#include <mysql.h>
#include "../include/base.h"

#define SERVER_VERSION_LENGTH 60

class ACL_SCHEMA;

class SERVER_CONNECTION_CNF:public mybase
{
public:

    SERVER_CONNECTION_CNF()
    {
    }

    SERVER_CONNECTION_CNF(const SERVER_CONNECTION_CNF& orig)
    {
    }

    virtual ~SERVER_CONNECTION_CNF()
    {
    }

    void set_server_version();
    char *get_server_version();
    int get_server_version_length();
    void set_user_acl(ACL_SCHEMA *schema);
    void set_charset_code(uint code);
    void set_trx(bool flag);
    bool get_using_trx();

    void set_max_client_packet_length(ulong length);
    unsigned int get_protocol_version();
    void set_protocol_version(unsigned int pv);

private:
    char server_version[SERVER_VERSION_LENGTH];
    int server_version_length;
    unsigned int protocol_version;
    bool using_transaction;
    ulong max_client_packet_length;
    uint charset_code;
    ACL_SCHEMA *user;
};

#endif	/* SERVER_CONNECTION_CNF_H */

