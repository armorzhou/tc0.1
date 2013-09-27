/* 
 * File:   mysql_native_password_auth.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月26日, 下午6:28
 */

#include <assert.h>
#include <mysql.h>
#include "mysql_native_password_auth.h"

typedef char ACL_USERS;
extern ACL_USERS *acl_user;

void *mysql_native_password_auth::mysql_acl_authenticate()
{
    
}

bool
mysql_native_password_auth::send_server_handshake_packet(
        struct session *ss, const char *data, uint32 data_len)
{
    NET *net = &ss->net;
    bool error;
    assert(data_len <= 255);

    char *buff = (char *) alloca(1 + SERVER_VERSION_LENGTH + data_len + 64);
    char scramble_buf[SCRAMBLE_LENGTH];
    char *end = buff;
    char scramble[SCRAMBLE_LENGTH];

    cout << "enter send_server_handshake_packet()" << endl;

    *end++ = protocol_version;
    ss->client_capabilities = CLIENT_BASIC_FLAGS;
    if (opt_using_transactions)
        ss->client_capabilities |= CLIENT_TRANSACTIONS;

    ss->client_capabilities |= CAN_CLIENT_COMPRESS;

    if (ssl_acceptor_fd)
    {
        ss->client_capabilities |= CLIENT_SSL;
        ss->client_capabilities |= CLIENT_SSL_VERIFY_SERVER_CERT;
    }

    if (data_len < SCRAMBLE_LENGTH)
    {
        if (data_len)
        {
            /*
              the first packet *must* have at least 20 bytes of a scramble.
              if a plugin provided less, we pad it to 20 with zeros
             */
            memcpy(scramble_buf, data, data_len);
            memset(scramble_buf + data_len, 0, SCRAMBLE_LENGTH - data_len);
            data = scramble_buf;
        }
        else
        {
            /*
              if the default plugin does not provide the data for the scramble at
              all, we generate a scramble internally anyway, just in case the
              user account (that will be known only later) uses a
              native_password_plugin (which needs a scramble). If we don't send a
              scramble now - wasting 20 bytes in the packet -
              native_password_plugin will have to send it in a separate packet,
              adding one more round trip.
             */
            create_random_string(scramble, SCRAMBLE_LENGTH, myrand);
            data = scramble;
        }
        data_len = SCRAMBLE_LENGTH;
    }

    end = strnmov(end, server_version, SERVER_VERSION_LENGTH) + 1;
    int4store((unsigned char*) end, (ulong)1986); //thread_id
    end += 4;

    /*
      Old clients does not understand long scrambles, but can ignore packet
      tail: that's why first part of the scramble is placed here, and second
      part at the end of packet.
     */
    end = (char*) memcpy(end, data, SCRAMBLE_LENGTH_323);
    end += SCRAMBLE_LENGTH_323;
    *end++ = 0;

    int2store(end, ss->client_capabilities);
    /* write server characteristics: up to 16 bytes allowed */
    end[2] = (char) 8; //character set number
    int2store(end + 3, 2);  //server_status[0]
    int2store(end + 5, ss->client_capabilities >> 16);
    end[7] = data_len;

    memset(end + 8, 0, 10);
    end += 18;
    
    /* write scramble tail */
    end = (char*) memcpy(end, data + SCRAMBLE_LENGTH_323,
            data_len - SCRAMBLE_LENGTH_323);
    end += data_len - SCRAMBLE_LENGTH_323;
    end = strmake(end, "mysql_native_password", strlen("mysql_native_password"));

    error = my_net_write(net, (uchar*) buff, (size_t) (end - buff + 1));
    
    if(error)
    {
        net_flush(net);
    }
    
    return error;
}
