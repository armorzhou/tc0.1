/* 
 * File:   login55.h
 * Author: tangchao
 *
 * Created on 2013年10月24日, 下午3:20
 */

#ifndef LOGIN55_H
#define	LOGIN55_H

#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <sys/syscall.h>
#include "../include/base.h"
#include "../acl/sql_acl.h"
#include "login.h"

using namespace std;

class LOGIN55 : public LOGIN, public mybase
{
public:
    LOGIN55(){ }
    LOGIN55(const LOGIN55& orig){ }
    virtual ~LOGIN55() { }
    
    void init(SERVER_CONNECTION_CNF* scc, PROTOCOL* pl);
    bool check_login();
    
    void net_get_normalized_ip(const struct sockaddr *src,
            int src_length, struct sockaddr *dst, int *dst_length);
    int net_getnameinfo(const struct sockaddr *sa, char *hostname,
            size_t hostname_size, char *port, size_t port_size,
            int flags);
    bool net_peer_addr();
    bool send_server_handshake(char *data, unsigned long data_len);
    ulong read_client_handshake_packet(unsigned char **buf);
    char* get_41_protocol_string(char **buffer, size_t *max_bytes_available,
            size_t *string_length);
    char * get_41_lenc_string(char **buffer, size_t *max_bytes_available,
            size_t *string_length);
    unsigned long parse_client_handshake_packet(unsigned char **buff, 
    unsigned long pkt_len);

    ulong get_tid();
    char* get_plugin_name();
    int get_plugin_name_length();
    char* get_server_version();
    unsigned long get_server_version_length();
    unsigned int get_protocol_version();

private:
    SERVER_CONNECTION_CNF *scc;
    PROTOCOL *pl;
    ACL_SCHEMA *user_acl;
    string username;
    char host[NI_MAXHOST];
    unsigned short port;
    bool localhost;
    struct sockaddr_storage local; /* Local internet address */
    struct sockaddr_storage remote; /* Remote internet address */
    int addrLen; /* Length of remote address */
    string pass;
};

#endif	/* LOGIN55_H */

