
/* 
 * File:   server_connection_cnf.h
 * Author: tangchao
 *
 * Created on 2013年10月24日, 下午4:24
 */
#include "server_connection_cnf.h"

void SERVER_CONNECTION_CNF::set_server_version()
{
    char *end = strxmov(this->server_version, MYSQL_SERVER_VERSION, "", NullS);
    if (!strstr("", "-debug"))
    {
        end = strmov(end, "-debug");
    }
    strmov(end, "-log");
    this->server_version_length = strlen(this->server_version);
    return;
}

char *SERVER_CONNECTION_CNF::get_server_version()
{
    return this->server_version;
}

int SERVER_CONNECTION_CNF::get_server_version_length()
{
    return this->server_version_length;
}

void SERVER_CONNECTION_CNF::set_user_acl(ACL_SCHEMA *schema)
{
    this->user = schema;
}

void SERVER_CONNECTION_CNF::set_charset_code(uint code)
{
    this->charset_code = code;
}

void SERVER_CONNECTION_CNF::set_trx(bool trx)
{
    this->using_transaction = trx;
}

bool SERVER_CONNECTION_CNF::get_using_trx()
{
    return this->get_using_trx();
}

void SERVER_CONNECTION_CNF::set_max_client_packet_length(ulong length)
{
    this->max_client_packet_length = length;
}

void SERVER_CONNECTION_CNF::set_protocol_version(unsigned int pv)
{
    this->protocol_version = pv;
}

void SERVER_CONNECTION_CNF::get_protocol_version()
{
    return this->protocol_version;
}
