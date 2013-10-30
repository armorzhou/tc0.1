/* 
 * File:   login55.cpp
 * Author: tangchao
 * 
 * Created on 2013年10月24日, 下午3:20
 */

#include <assert.h>
#include <mysql.h>
#include "../log/jlog.h"
#include "../protocol/protocol_10.h"
#include "../include/byte_order_genric.h"
#include "../route/handle_zk.h"
#include "login55.h"

extern struct rand_struct password_rand;

void
LOGIN55::init(SERVER_CONNECTION_CNF* scc, PROTOCOL* pl)
{
    this->scc = scc;
    this->pl = pl;
    return;
}

ulong LOGIN55::get_tid()
{
    return syscall(__NR_gettid);

}

char *LOGIN55::get_plugin_name()
{
    return "mysql_native_password";
}

int LOGIN55::get_plugin_name_length()
{
    return strlen(this->get_plugin_name());
}

char* LOGIN55::get_server_version()
{
    return this->scc->get_server_version();
}

unsigned long LOGIN55::get_server_version_length()
{
    return this->scc->get_server_version_length();
}

unsigned int LOGIN55::get_protocol_version()
{
    return this->scc->get_protocol_version();
}

void
LOGIN55::net_get_normalized_ip(const struct sockaddr *src,
        int src_length,
        struct sockaddr *dst,
        int *dst_length)
{
    switch (src->sa_family)
    {
        case AF_INET:
            memcpy(dst, src, src_length);
            *dst_length = src_length;
            break;

        case AF_INET6:
        {
            const struct sockaddr_in6 *src_addr6 = (const struct sockaddr_in6 *) src;
            const struct in6_addr *src_ip6 = &(src_addr6->sin6_addr);
            const uint32 *src_ip6_int32 = (uint32 *) src_ip6->s6_addr;

            if (IN6_IS_ADDR_V4MAPPED(src_ip6) || IN6_IS_ADDR_V4COMPAT(src_ip6))
            {
                struct sockaddr_in *dst_ip4 = (struct sockaddr_in *) dst;

                /*
                  This is an IPv4-mapped or IPv4-compatible IPv6 address. It should
                  be converted to the IPv4 form.
                 */

                *dst_length = sizeof (struct sockaddr_in);

                memset(dst_ip4, 0, *dst_length);
                dst_ip4->sin_family = AF_INET;
                dst_ip4->sin_port = src_addr6->sin6_port;

                /*
                  In an IPv4 mapped or compatible address, the last 32 bits represent
                  the IPv4 address. The byte orders for IPv6 and IPv4 addresses are
                  the same, so a simple copy is possible.
                 */
                dst_ip4->sin_addr.s_addr = src_ip6_int32[3];
            }
            else
            {
                /* This is a "native" IPv6 address. */

                memcpy(dst, src, src_length);
                *dst_length = src_length;
            }

            break;
        }
    }
    return;
}

int
LOGIN55::net_getnameinfo(const struct sockaddr *sa,
        char *hostname, size_t hostname_size,
        char *port, size_t port_size,
        int flags)
{
    int sa_length = 0;

    switch (sa->sa_family)
    {
        case AF_INET:
            sa_length = sizeof (struct sockaddr_in);
#ifdef HAVE_SOCKADDR_IN_SIN_LEN
            ((struct sockaddr_in *) sa)->sin_len = sa_length;
#endif /* HAVE_SOCKADDR_IN_SIN_LEN */
            break;

#ifdef HAVE_IPV6
        case AF_INET6:
            sa_length = sizeof (struct sockaddr_in6);
#ifdef HAVE_SOCKADDR_IN6_SIN6_LEN
            ((struct sockaddr_in6 *) sa)->sin6_len = sa_length;
#endif /* HAVE_SOCKADDR_IN6_SIN6_LEN */
            break;
#endif /* HAVE_IPV6 */
    }

    return getnameinfo(sa, sa_length,
            hostname, hostname_size,
            port, port_size,
            flags);
}

bool
LOGIN55::net_peer_addr()
{
    if (this->localhost)
    {
        /*
          Initialize net->remote and net->addLen. Set net->remote to IPv4 loopback
          address.
         */
        struct in_addr *ip4 = &((struct sockaddr_in *) &(this->remote))->sin_addr;
        this->remote.ss_family = AF_INET;
        this->addrLen = sizeof (struct sockaddr_in);
        ip4->s_addr = htonl(INADDR_LOOPBACK);
        /* Initialize ip_buffer and port. */
        strmov(this->host, "127.0.0.1");
        this->port = 0;
    }
    else
    {
        int err_code;
        char port_buffer[NI_MAXSERV];
        struct sockaddr_storage addr_storage;
        struct sockaddr *addr = (struct sockaddr *) &addr_storage;
        socklen_t addr_length = sizeof (addr_storage);
        /* Get sockaddr by socked fd. */
        err_code = getpeername(this->pl->get_socket(), addr, &addr_length);
        if (err_code)
        {
            jlog(WARNING, "exit getpeername() gave error: %d", socket_errno);
            return false;
        }
        /* Normalize IP address. */
        net_get_normalized_ip(addr, addr_length,
                (struct sockaddr *) & this->remote, &this->addrLen);
        /* Get IP address & port number. */
        err_code = net_getnameinfo((struct sockaddr *) & this->remote,
                this->host, NI_MAXHOST,
                port_buffer, NI_MAXSERV,
                NI_NUMERICHOST | NI_NUMERICSERV);
        if (err_code)
        {
            jlog(WARNING, "exit getnameinfo() gave error: %s",
                    gai_strerror(err_code));
            return false;
        }
        this->port = (uint16) strtol(port_buffer, NULL, 10);
    }
    jlog(INFO, "exit Client IP address: %s; port: %d", this->host, this->port);
    return true;
}

char*
LOGIN55::get_41_protocol_string(char **buffer, size_t *max_bytes_available,
        size_t *string_length)
{
    char *str = (char *) memchr(*buffer, '\0', *max_bytes_available);
    if (str == NULL)
    {
        return NULL;
    }

    *string_length = (size_t) (str - *buffer);
    *max_bytes_available -= *string_length + 1;
    str = *buffer;
    *buffer += *string_length + 1;

    return str;
}

char *
LOGIN55::get_41_lenc_string(char **buffer,
        size_t *max_bytes_available,
        size_t *string_length)
{
    if (*max_bytes_available == 0)
        return NULL;

    /* Do double cast to prevent overflow from signed / unsigned conversion */
    size_t str_len = (size_t) (unsigned char) **buffer;

    /*
      If the length encoded string has the length 0
      the total size of the string is only one byte long (the size byte)
     */
    if (str_len == 0)
    {
        ++*buffer;
        *string_length = 0;
        /*
          Return a pointer to the 0 character so the return value will be
          an empty string.
         */
        return *buffer - 1;
    }

    if (str_len >= *max_bytes_available)
        return NULL;

    char *str = *buffer + 1;
    *string_length = str_len;
    *max_bytes_available -= *string_length + 1;
    *buffer += *string_length + 1;
    return str;
}

unsigned long
LOGIN55::parse_client_handshake_packet(unsigned char **buff,
        unsigned long pkt_len)
{
    char *end;
    char *db = NULL;
    size_t passwd_len = 0;
    char *passwd = NULL;
    bool packet_has_required_size = false;
    unsigned long client_capabilities;

    size_t db_len = 0;
    end = (char *) this->pl->get_read_pos();

    size_t bytes_remaining_in_packet = pkt_len;

    if (bytes_remaining_in_packet <  2)
    {
        jlog(WARNING, "client error.");
        return packet_error;
    }

    client_capabilities = uint2korr(end);

    /* JConnector only sends server capabilities before starting SSL
         negotiation.  The below code is patch for this. */
    if (bytes_remaining_in_packet == 4 && client_capabilities & CLIENT_SSL)
    {
        jlog(INFO, "JConnector");
        client_capabilities = uint4korr(end);
        this->scc->set_max_client_packet_length(0xfffff);
        goto skip_to_normal;
    }

    if (client_capabilities & CLIENT_PROTOCOL_41)
    {
        packet_has_required_size = bytes_remaining_in_packet >=
                AUTH_PACKET_HEADER_SIZE_PROTO_41;
    }
    else
        return packet_error;

    if (!packet_has_required_size)
        return packet_error;

    if (client_capabilities & CLIENT_PROTOCOL_41)
    {
        client_capabilities = uint4korr(end);
        this->scc->set_max_client_packet_length(uint4korr(end + 4));
        this->scc->set_charset_code((uint) (uchar)*(end + 8));
        /*
          Skip 23 remaining filler bytes which have no particular meaning.
         */
        end += AUTH_PACKET_HEADER_SIZE_PROTO_41;
        bytes_remaining_in_packet -= AUTH_PACKET_HEADER_SIZE_PROTO_41;
    }
    else
        return packet_error;

skip_to_normal:
    if (client_capabilities & CLIENT_TRANSACTIONS
            && this->scc->get_using_trx())
    {
        jlog(INFO, "client use begin (transaction)");
        this->scc->set_trx(true);
    }

    if (client_capabilities & CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA &&
            !client_capabilities & CLIENT_SECURE_CONNECTION)
        return packet_error;

    bytes_remaining_in_packet = pkt_len - (end - (char *) this->pl->get_read_pos());

    size_t user_len;
    char *user = get_41_protocol_string(&end, &bytes_remaining_in_packet, &user_len);
    if (user == NULL)
    {
        return packet_error;
    }
    jlog(INFO, "login user:%s", user);
    this->username = user;

    /*
      Old clients send a null-terminated string as password; new clients send
      the size (1 byte) + string (not null-terminated). Hence in case of empty
      password both send '\0'.
     */

    if (client_capabilities & CLIENT_SECURE_CONNECTION)
    {
        passwd = get_41_lenc_string(&end, &bytes_remaining_in_packet,
                &passwd_len);
    }
    else
    {
        jlog(WARNING, "client have no password.");
        return packet_error;
    }

    if (passwd == NULL)
    {
        return packet_error;
    }
    jlog(INFO, "login password:%s", passwd);
    this->pass = passwd;
    
    if (client_capabilities & CLIENT_CONNECT_WITH_DB)
    {
        db = get_41_protocol_string(&end, &bytes_remaining_in_packet, &db_len);
        if (db == NULL)
            return packet_error;
    }

    SCHEMA schema = SCHEMA(this->username, this->host);
    this->user_acl = SQL_ACL::get_instance(false)->find_user(schema);
    if (NULL == this->user_acl)
    {
        jlog(WARNING, "no user");
        return packet_error;
    }
    *buff = (uchar*) passwd;
    return passwd_len;
}

ulong
LOGIN55::read_client_handshake_packet(unsigned char **buf)
{
    ulong pkt_len = 0;

    pkt_len = this->pl->net_read_data();
    if (pkt_len == packet_error)
    {
        jlog(WARNING, "read error.");
        return packet_error;
    }

    this->pl->read_packets_plus();
    jlog(INFO, "read packets counter %lu", this->pl->get_read_packets());
    /* The 1st packet has the plugin data wrapped into the client authentication
      handshake packet. */
    if (this->pl->get_read_packets() == 1)
    {
        pkt_len = parse_client_handshake_packet(buf, pkt_len);
        if (pkt_len == packet_error)
        {
            return packet_error;
        }
    }
    else
        *buf = this->pl->get_read_pos();

    return pkt_len;
}

/*
  Packet format:
   
    Bytes       Content
    -----       ----
    1           protocol version (always 10)
    n           server version string, \0-terminated
    4           thread id
    8           first 8 bytes of the plugin provided data (scramble)
    1           \0 byte, terminating the first part of a scramble
    2           server capabilities (two lower bytes)
    1           server character set
    2           server status
    2           server capabilities (two upper bytes)
    1           length of the scramble
    10          reserved, always 0
    n           rest of the plugin provided data (at least 12 bytes)
    1           \0 byte, terminating the second part of a scramble
 */
bool
LOGIN55::send_server_handshake(char *data, unsigned long data_len)
{  
    assert(data_len == SCRAMBLE_LENGTH + 1);
    unsigned long client_capabilities;
    char *end;
    char *buff = (char *) alloca(1 + SERVER_VERSION_LENGTH + data_len + 64);
    end = buff;

    *end++ = (unsigned int)10;
    client_capabilities = CLIENT_BASIC_FLAGS;
    /* close auto commit */
    //client_capabilities |= CLIENT_TRANSACTIONS;
    /* close compress */
    //client_capabilities |= CAN_CLIENT_COMPRESS;

    end = strnmov(end, "jd-2.0.1-dev", SERVER_VERSION_LENGTH) + 1;
    /*thread id */
    int4store((unsigned char*) end, (ulong) 1986);
    end += 4;

    end = (char*) memcpy(end, data, SCRAMBLE_LENGTH_323);
    end += SCRAMBLE_LENGTH_323;
    *end++ = 0;

    /* server options */
    int2store(end, client_capabilities);
    /* default character set number */
    end[2] = (char) 8;
    /* server status */
    int2store(end + 3, 2);
    int2store(end + 5, client_capabilities >> 16);
    end[7] = data_len;
    memset(end + 8, 0, 10);
    end += 18;

    /* write scramble tail */
    end = (char*) memcpy(end, data + SCRAMBLE_LENGTH_323,
            data_len - SCRAMBLE_LENGTH_323);
    end += data_len - SCRAMBLE_LENGTH_323;
    end = strmake(end, "http://www.jd.com", strlen("http://www.jd.com"));

    if (this->pl->net_write_data((uchar*) buff, (size_t) (end - buff + 1)))
    {
        return this->pl->net_flush();
    }
    return false;
}


bool
LOGIN55::check_login()
{
    unsigned char *pkt = NULL;
    char scramble[SCRAMBLE_LENGTH];

    create_random_string(scramble, SCRAMBLE_LENGTH, &password_rand);

    /* send first packet to the client */
    if (false == this->send_server_handshake(scramble, SCRAMBLE_LENGTH + 1))
        return false;

    sleep(2);
    /* read auth info from client */
    if (this->read_client_handshake_packet(&pkt) == packet_error)
        return false;

    jlog(INFO, "scramble:%s, salt:%s", scramble,
            this->user_acl->get_salt());
    /* check password */
    if (check_scramble(pkt, scramble, this->user_acl->get_salt()))
    {
        pl->net_send_error(8888, "authenticate failed.", "HY000");
        return false;
    }

    return pl->net_send_ok(2, 0, 0, 0, '\0');
}
