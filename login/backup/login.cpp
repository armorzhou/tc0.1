/* Author: tangchao
 * Email: tangchao@jd.com
 */

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <iconv.h>
#include <stddef.h>

#include "../log/jlog.h"
#include "../acl/sql_acl.h"
#include "../session.h"
#include "../server_version.h"
#include "../protocol/net.h"
#include "../protocol/protocol.h"
#include "../base/base.h"
#include "../ctype.h"
#include "../tc_global.h"
#include "password.h"
#include "byte_order_genric.h"
#include "login.h"

/* Local functions */
static bool get_client_addr(TC_NET *net, char *ip_buffer, uint16 *port,
        size_t ip_buffer_size);
static bool login_acl_check_host(char *ip);
static bool send_server_handshake_packet(class session *ss,
        const char *data, unsigned int data_len);
static bool read_client_connect_attrs(char **ptr, size_t *max_bytes_available);
static int server_mpvio_read_packet(class session *ss, unsigned char **buf);
unsigned long long net_field_length_ll(uchar **packet);
static char* get_41_protocol_string(char **buffer, size_t *max_bytes_available,
        size_t *string_length);
static char * get_40_protocol_string(char **buffer, size_t *max_bytes_available,
        size_t *string_length);
static char * get_56_lenc_string(char **buffer, size_t *max_bytes_available,
        size_t *string_length);
static char * get_41_lenc_string(char **buffer, size_t *max_bytes_available,
        size_t *string_length);
static unsigned long parse_client_handshake_packet(class session *ss,
        unsigned char **buff, unsigned long pkt_len);
static int native_password_authenticate(class session *ss, char *scramble);
static bool acl_authenticate(class session *ss);

/**************************************************************
  Function:     tc_convert
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
int
tc_convert(char *outbuf, size_t outlen, char *inbuf, size_t inlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    size_t res = 0;

    jlog(INFO, "enter tc_convert");

    cd = iconv_open("UTF-8", "ASCII");
    if (cd == (iconv_t) - 1)
    {
        jlog(ERROR, "iconv open error");
        return -1;
    }

    memset(outbuf, 0, outlen);
    res = iconv(cd, pin, &inlen, pout, &outlen);

    if (res == (size_t) - 1)
    {
        jlog(ERROR, "convert error");
    }

    jlog(INFO, "res = %lu", res);
    iconv_close(cd);
    return res;
}

/**************************************************************
  Function:     tc_copy_and_convert
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
int
tc_copy_and_convert(char *to, size_t to_length, char *from, size_t from_length)
{
    return tc_convert(to, to_length, from, from_length);
}

/**************************************************************
  Function:     get_client_addr
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
static bool
get_client_addr(TC_NET *net, char *ip_buffer, uint16 *port, size_t ip_buffer_size)
{
    jlog(INFO, "enter get_client_addr");
    return net_peer_addr(net, ip_buffer, port, ip_buffer_size);
}

/**************************************************************
  Function:     login_acl_check_host
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
bool
login_acl_check_host(char *ip)
{
    jlog(INFO, "enter login_acl_check_host");
    return acl->acl_check_host(ip);
}

/**************************************************************
  Function:     send_server_handshake_packet
  Description:    sends a server handshake initialization packet, the very first packet
  after the connection was established

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
  Author: 
  Date:         
  Input: 
  Output: 
  Return:   @retval true ok
  @retval false error
  Other:
 **************************************************************/
static bool
send_server_handshake_packet(class session *ss, const char *data,
        unsigned int data_len)
{
    TC_NET *net = ss->get_net();
    bool error;
    assert(data_len <= 255);

    char *buff = (char *) alloca(1 + SERVER_VERSION_LENGTH + data_len + 64);
    char scramble_buf[SCRAMBLE_LENGTH];
    char *end = buff;
    char scramble[SCRAMBLE_LENGTH];

    jlog(INFO, "enter send_server_handshake_packet()");

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
            tc_create_random_string(scramble, SCRAMBLE_LENGTH, &password_rand);
            data = scramble;
        }
        data_len = SCRAMBLE_LENGTH;
    }

    end = strnmov(end, server_version, SERVER_VERSION_LENGTH) + 1;
    int4store((unsigned char*) end, (ulong) 1986); //thread_id
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
    int2store(end + 3, 2); //server_status[0]
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

    if (error)
    {
        net_flush(net);
    }

    return error;
}

/**************************************************************
  Function:     read_client_connect_attrs
  Description:  
  This function is called by a server authentication plugin, when it wants
  to read data from the client.

  It transparently extracts the client plugin data, if embedded into
  a client authentication handshake packet, and handles plugin negotiation
  with the client, if necessary.
  Author: 
  Date:         
  Input: 
  Output: 
  Return:true ok, false error.
  Other:
 **************************************************************/
static bool
read_client_connect_attrs(char **ptr, size_t *max_bytes_available)
{
    size_t length, length_length;
    char *ptr_save;

    jlog(INFO, "enter read_client_connect_attrs");

    /* not enough bytes to hold the length */
    if (*max_bytes_available < 1)
    {
        jlog(ERROR, "not enough bytes to hold the length");
        return false;
    }

    /* read the length */
    ptr_save = *ptr;
    length = net_field_length_ll((uchar **) ptr);
    length_length = *ptr - ptr_save;
    if (*max_bytes_available < length_length)
    {
        jlog(ERROR, "greater than max_bytes_available");
        return false;
    }

    *max_bytes_available -= length_length;

    /* length says there're more data than can fit into the packet */
    if (length > *max_bytes_available)
    {
        jlog(ERROR, "greater than max_bytes_available");
        return false;
    }

    /* impose an artificial length limit of 64k */
    if (length > 65535)
    {
        jlog(ERROR, "greater than 64k");
        return false;
    }

    return true;
}

/**************************************************************
  Function:     server_mpvio_read_packet
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return:  
  Other:
 **************************************************************/
static int
server_mpvio_read_packet(class session *ss, unsigned char **buf)
{
    ulong pkt_len = 0;
    TC_NET *net = ss->net;

    jlog(INFO, "enter server_mpvio_read_packet, sd=%d", net->sd);

    pkt_len = my_net_read(net);

    if (pkt_len == packet_error)
    {
        jlog(ERROR, "pkt_len:%lu", packet_error);
        return -1;
    }

    net->packets_read++;
    /*
      the 1st packet has the plugin data wrapped into the client authentication
      handshake packet
     */
    if (net->packets_read == 1)
    {
        pkt_len = parse_client_handshake_packet(ss, buf, pkt_len);
        if (pkt_len == packet_error)
        {
            jlog(ERROR, "parse client error");
            return -1;
        }
    }
    else
    {
        *buf = net->read_pos;
    }

    return ((int) pkt_len);
}

/**************************************************************
  Function:     net_field_length_ll
  Description:  The same as above but returns longlong 
  Author: 
  Date:         
  Input: 
  Output: 
  Return:  
  Other:
 **************************************************************/
unsigned long long
net_field_length_ll(uchar **packet)
{
    register uchar *pos = *packet;
    if (*pos < 251)
    {
        (*packet)++;
        return (unsigned long long) *pos;
    }
    if (*pos == 251)
    {
        (*packet)++;
        return (unsigned long long) NULL_LENGTH;
    }
    if (*pos == 252)
    {
        (*packet) += 3;
        return (unsigned long long) uint2korr(pos + 1);
    }
    if (*pos == 253)
    {
        (*packet) += 4;
        return (unsigned long long) uint3korr(pos + 1);
    }
    (*packet) += 9; /* Must be 254 when here */

    return (unsigned long long) uint8korr(pos + 1);
}

/** Get a string according to the protocol of the underlying buffer. */
typedef char * (*get_proto_string_func_t) (char **, size_t *, size_t *);

/**************************************************************
  Function:     get_41_protocol_string
  Description:    Get a string formatted according to the 4.1 version of 
 * the MySQL protocol.
  Author: 
  Date:
  Input:
  @param buffer[in, out]    Pointer to the user-supplied buffer to be scanned.
  @param max_bytes_available[in, out]  Limit the bytes to scan.
  @param string_length[out] The number of characters scanned not including
                            the null character.

  @remark Strings are always null character terminated in this version of the
          protocol.

  @remark The string_length does not include the terminating null character.
          However, after the call, the buffer is increased by string_length+1
          bytes, beyond the null character if there still available bytes to
          scan.
  Output: 
  Return:  
    @return pointer to beginning of the string scanned.
    @retval NULL The buffer content is malformed
  Other:
 **************************************************************/
static char*
get_41_protocol_string(char **buffer,
        size_t *max_bytes_available,
        size_t *string_length)
{
    char *str = (char *) memchr(*buffer, '\0', *max_bytes_available);

    if (str == NULL)
        return NULL;

    *string_length = (size_t) (str - *buffer);
    *max_bytes_available -= *string_length + 1;
    str = *buffer;
    *buffer += *string_length + 1;

    return str;
}

/**************************************************************
  Function:     get_40_protocol_string
  Description:      Get a string formatted according to the 4.0 version of the MySQL protocol.
  Author: 
  Date:         
  Input:   
  @param buffer[in, out]    Pointer to the user-supplied buffer to be scanned.
  @param max_bytes_available[in, out]  Limit the bytes to scan.
  @param string_length[out] The number of characters scanned not including
                            the null character.

  @remark If there are not enough bytes left after the current position of
          the buffer to satisfy the current string, the string is considered
          to be empty and a pointer to empty_c_string is returned.

  @remark A string at the end of the packet is not null terminated.
  Output: 
  Return:  @return Pointer to beginning of the string scanned, or a pointer to a empty
          string.
  Other:
 **************************************************************/
static char *
get_40_protocol_string(char **buffer,
        size_t *max_bytes_available,
        size_t *string_length)
{
    char *str;
    size_t len;

    /* No bytes to scan left, treat string as empty. */
    if ((*max_bytes_available) == 0)
    {
        *string_length = 0;
        return empty_c_string;
    }

    str = (char *) memchr(*buffer, '\0', *max_bytes_available);

    /*
      If the string was not null terminated by the client,
      the remainder of the packet is the string. Otherwise,
      advance the buffer past the end of the null terminated
      string.
     */
    if (str == NULL)
        len = *string_length = *max_bytes_available;
    else
        len = (*string_length = (size_t) (str - *buffer)) + 1;

    str = *buffer;
    *buffer += len;
    *max_bytes_available -= len;

    return str;
}

/**************************************************************
  Function:     get_56_lenc_string
  Description:  Get a length encoded string from a user-supplied buffer.
  Author: 
  Date:         
  Input:   
  @param buffer[in, out] The buffer to scan; updates position after scan.
  @param max_bytes_available[in, out] Limit the number of bytes to scan
  @param string_length[out] Number of characters scanned

  @remark In case the length is zero, then the total size of the string is
    considered to be 1 byte; the size byte.
  Output: 
  Return:    @return pointer to first byte after the header in buffer.
    @retval NULL The buffer content is malformed
  Other:
 **************************************************************/
static char *
get_56_lenc_string(char **buffer,
        size_t *max_bytes_available,
        size_t *string_length)
{
    static char empty_string[1] = {'\0'};
    char *begin = *buffer;

    if (*max_bytes_available == 0)
        return NULL;

    /*
      If the length encoded string has the length 0
      the total size of the string is only one byte long (the size byte)
     */
    if (*begin == 0)
    {
        *string_length = 0;
        --*max_bytes_available;
        ++*buffer;
        /*
          Return a pointer to the \0 character so the return value will be
          an empty string.
         */
        return empty_string;
    }

    *string_length = (size_t) net_field_length_ll((uchar **) buffer);

    size_t len_len = (size_t) (*buffer - begin);

    if (*string_length + len_len > *max_bytes_available)
        return NULL;

    *max_bytes_available -= *string_length + len_len;
    *buffer += *string_length;
    return (char *) (begin + len_len);
}

/**************************************************************
  Function:     get_41_lenc_string
  Description:   Get a length encoded string from a user-supplied buffer.
  Author: 
  Date:         
  Input:   
  @param buffer[in, out] The buffer to scan; updates position after scan.
  @param max_bytes_available[in, out] Limit the number of bytes to scan
  @param string_length[out] Number of characters scanned

  @remark In case the length is zero, then the total size of the string is
    considered to be 1 byte; the size byte.

  @note the maximum size of the string is 255 because the header is always 
    1 byte.
  Output: 
  Return:     @return pointer to first byte after the header in buffer.
    @retval NULL The buffer content is malformed
  Other:
 **************************************************************/
static char *
get_41_lenc_string(char **buffer,
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

/**************************************************************
  Function:     parse_client_handshake_packet
  Description:   the packet format is described in send_client_reply_packet()
  Author: 
  Date:         
  Input:   
  Output: 
  Return: 
  Other:
 **************************************************************/
static unsigned long
parse_client_handshake_packet(class session *ss, unsigned char **buff, unsigned long pkt_len)
{
    char *end;
    size_t passwd_len = 0;
    char *passwd = NULL;
    bool packet_has_required_size = false;
    TC_NET *net = ss->get_net();

    jlog(INFO, "enter parse_client_handshake_packet");

    size_t db_len = 0;
    char *db = NULL;

    end = (char *) net->read_pos;
    /*
      In order to safely scan a head for '\0' string terminators
      we must keep track of how many bytes remain in the allocated
      buffer or we might read past the end of the buffer.
     */
    size_t bytes_remaining_in_packet = pkt_len;

    /*
      Peek ahead on the client capability packet and determine which version of
      the protocol should be used.
     */
    if (bytes_remaining_in_packet < 2)
    {
        jlog(ERROR, "bytes_remaining_in_packet:%lu", bytes_remaining_in_packet);
        return packet_error;
    }

    ulong client_capabilities = uint2korr(end);
    jlog(INFO, "client capabilities:%lu, server capabilities:%lu",
            client_capabilities, ss->client_capabilities);
    ss->client_capabilities = client_capabilities;

    /*
         JConnector only sends server capabilities before starting SSL
         negotiation.  The below code is patch for this.
     */
    if (bytes_remaining_in_packet == 4 && ss->client_capabilities & CLIENT_SSL)
    {
        ss->set_client_capabilities(uint4korr(end));
        ss->set_max_client_packet_length(0xfffff);
        goto skip_to_normal;
    }

    if (ss->client_capabilities & CLIENT_PROTOCOL_41)
    {
        jlog(INFO, "CLIENT_PROTOCOL_41");

        packet_has_required_size = bytes_remaining_in_packet >=
                AUTH_PACKET_HEADER_SIZE_PROTO_41;
    }
    else
    {
        jlog(ERROR, "do not support");
        packet_has_required_size = bytes_remaining_in_packet >=
                AUTH_PACKET_HEADER_SIZE_PROTO_40;
    }


    if (!packet_has_required_size)
    {
        jlog(ERROR, "packet_has_required_size:%u", packet_has_required_size);
        return packet_error;
    }


    if (ss->client_capabilities & CLIENT_PROTOCOL_41)
    {
        ss->client_capabilities = uint4korr(end);
        ss->max_client_packet_length = uint4korr(end + 4);
        ss->charset_code = (uint) (uchar)*(end + 8);
        /*
          Skip 23 remaining filler bytes which have no particular meaning.
         */
        end += AUTH_PACKET_HEADER_SIZE_PROTO_41;
        bytes_remaining_in_packet -= AUTH_PACKET_HEADER_SIZE_PROTO_41;
    }
    else
    {
        ss->client_capabilities = uint2korr(end);
        ss->max_client_packet_length = uint3korr(end + 2);
        end += AUTH_PACKET_HEADER_SIZE_PROTO_40;
        bytes_remaining_in_packet -= AUTH_PACKET_HEADER_SIZE_PROTO_40;
        /**
          Old clients didn't have their own charset. Instead the assumption
          was that they used what ever the server used.
         */
        ss->charset_code = 0;
    }

skip_to_normal:

    if (ss->client_capabilities & CLIENT_TRANSACTIONS
            && opt_using_transactions)
    {
        jlog(INFO, "client use begin (transaction)");
        ss->xa_transaction = true;
    }

    /*
      The 4.0 and 4.1 versions of the protocol differ on how strings
      are terminated. In the 4.0 version, if a string is at the end
      of the packet, the string is not null terminated. Do not assume
      that the returned string is always null terminated.
     */
    get_proto_string_func_t get_string;

    if (ss->client_capabilities & CLIENT_PROTOCOL_41)
    {
        jlog(INFO, "CLIENT_PROTOCOL_41");
        get_string = get_41_protocol_string;
    }
    else
    {
        jlog(INFO, "CLIENT_PROTOCOL_40");
        get_string = get_40_protocol_string;
    }

    /*
      When the ability to change default plugin require that the initial password
     field can be of arbitrary size. However, the 41 client-server protocol limits
     the length of the auth-data-field sent from client to server to 255 bytes
     (CLIENT_SECURE_CONNECTION). The solution is to change the type of the field
     to a true length encoded string and indicate the protocol change with a new
     client capability flag: CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA.
     */
    get_proto_string_func_t get_length_encoded_string;
#if 0
    if (ss->client_capabilities & CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA)
    {
        jlog(INFO, "password version 56");
        get_length_encoded_string = get_56_lenc_string;
    }
    else
    {
        jlog(INFO, "password version 41");
        get_length_encoded_string = get_41_lenc_string;

    }
#else
    jlog(INFO, "password version 41");
    get_length_encoded_string = get_41_lenc_string;
#endif

    /*
      The CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA capability depends on the
      CLIENT_SECURE_CONNECTION. Refuse any connection which have the first but
      not the latter.
     */
    if (ss->client_capabilities & CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA &&
            !ss->client_capabilities & CLIENT_SECURE_CONNECTION)
    {
        jlog(ERROR, "CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA ,CLIENT_SECURE_CONNECTION");
        return packet_error;
    }

    /*
      In order to safely scan a head for '\0' string terminators
      we must keep track of how many bytes remain in the allocated
      buffer or we might read past the end of the buffer.
     */
    bytes_remaining_in_packet = pkt_len - (end - (char *) net->read_pos);

    size_t user_len;
    char *user = get_string(&end, &bytes_remaining_in_packet, &user_len);
    if (user == NULL)
    {
        jlog(ERROR, "user:%s", user);
        return packet_error;
    }
    jlog(INFO, "user:%s", user);

    /*
      Old clients send a null-terminated string as password; new clients send
      the size (1 byte) + string (not null-terminated). Hence in case of empty
      password both send '\0'.
     */

    if (ss->get_client_capabilities() & CLIENT_SECURE_CONNECTION)
    {
        /*
          Get the password field.
         */
        jlog(INFO, "read password");
        passwd = get_length_encoded_string(&end, &bytes_remaining_in_packet,
                &passwd_len);
    }
    else
    {
        /*
          Old passwords are zero terminated strings.
         */
        jlog(INFO, "read old password.");
        passwd = get_string(&end, &bytes_remaining_in_packet, &passwd_len);
    }

    if (passwd == NULL)
    {
        jlog(WARNING, "password:%s", passwd);
        return packet_error;
    }
    jlog(INFO, "password:%s", passwd);

    if (ss->get_client_capabilities() & CLIENT_CONNECT_WITH_DB)
    {
        db = get_string(&end, &bytes_remaining_in_packet, &db_len);
        if (db == NULL)
        {
            jlog(INFO, "db:%s", db);
            return packet_error;
        }
    }
    jlog(INFO, "db:%s", db);

    /*
      Set the default for the password supplied flag for non-existing users
      as the default plugin (native passsword authentication) would do it
      for compatibility reasons.
     */
    size_t client_plugin_len = 0;
    char *client_plugin = get_string(&end, &bytes_remaining_in_packet,
            &client_plugin_len);
    jlog(INFO, "client plugin:%s", client_plugin);

    if (client_plugin == NULL)
    {
        client_plugin = &empty_c_string[0];
    }

    if ((ss->client_capabilities & CLIENT_CONNECT_ATTRS) &&
            !read_client_connect_attrs(&end, &bytes_remaining_in_packet))
    {
        jlog(ERROR, "see http://dev.mysql.com/doc/refman/5.6/en/mysql-options4.html");
    }

    ss->set_db(db);
    ss->set_user(user);

    SCHEMA schema = SCHEMA(ss->get_user(), ss->get_host());
    ACL_SCHEMA *acl_user = acl->find_user(&schema);
    if (NULL == acl_user)
    {
        jlog(INFO, "%s@%s not found", ss->get_user().c_str(), ss->get_host().c_str());
        return packet_error;
    }

    jlog(INFO, "find user %s@%s", ss->get_user().c_str(), ss->get_host().c_str());
    ss->set_user_acl(acl_user);

    if (!(ss->client_capabilities & CLIENT_PLUGIN_AUTH))
    {
        /*
          An old client is connecting
         */
        jlog(INFO, "old client.");
        if (ss->client_capabilities & CLIENT_SECURE_CONNECTION)
            client_plugin = "mysql_native_password";
        else
        {
            /*
              A really old client is connecting
             */
            client_plugin = "mysql_old_password";
            /*
              For a passwordless accounts we use native_password_plugin.
              But when an old 4.0 client connects to it, we change it to
              old_password_plugin, otherwise MySQL will think that server 
              and client plugins don't match.
             */
            jlog(INFO, "client driver error.");
            return -1;
        }
    }

    *buff = (uchar*) passwd;
    jlog(INFO, "password_len=%lu", passwd_len);
    return passwd_len;
}

/**************************************************************
  Function:     native_password_authenticate
  Description:    MySQL Server Password Authentication Plugin

  In the MySQL authentication protocol:
  1. the server sends the random scramble to the client
  2. client sends the encrypted password back to the server
  3. the server checks the password.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: check success return true, if a valid privilege return fasle
  Other:
 **************************************************************/
static int
native_password_authenticate(class session *ss, char *scramble)
{
    unsigned char *pkt = NULL;
    int res;

    jlog(INFO, "enter native_password_authenticate");

    tc_create_random_string(scramble, SCRAMBLE_LENGTH, &password_rand);

    /* send it to the client */
    if (false == send_server_handshake_packet(ss, scramble, SCRAMBLE_LENGTH + 1))
    {
        jlog(ERROR, "send to client error");
        return false;
    }

    if (server_mpvio_read_packet(ss, &pkt) < 0)
    {
        jlog(ERROR, "read client packet error.");
        return CR_AUTH_USER_CREDENTIALS;
    }

    ACL_SCHEMA *acl_tmp = ss->get_user_acl();
    res = (check_scramble(pkt, scramble, acl_tmp->get_salt()) ?
            CR_AUTH_USER_CREDENTIALS : CR_OK);
    jlog(INFO, "check pass res :%d[-1]", res);
    return res;
}

/**************************************************************
  Function:     tc_acl_authenticate
  Description:   Perform the handshake, authorize the client and update thd sctx variables
  Author: 
  Date:         
  Input:     @param ss                      session
                @param acl acl
               @param  sock_fd socket
  Output: 
  Return:   @retval 0  success, thd is updated.  @retval 1  error
  Other:
 **************************************************************/
static bool
acl_authenticate(class session *ss)
{
    int res = CR_OK;
    char msg[MYSQL_ERRMSG_SIZE];
    /* mark the thd as having no scramble yet */
    char scramble[SCRAMBLE_LENGTH];
    scramble[SCRAMBLE_LENGTH] = 1;

    /*
     perform the first authentication attempt, with the default plugin.
     This sends the server handshake packet, reads the client reply
     with a user name, and performs the authentication if everyone has used
     the correct plugin.
     */
    res = native_password_authenticate(ss, scramble);

    if (res > -1)
    {
        net_send_error_packet(ss, 8888, "authenticate failed.", "HY000");
        jlog(ERROR, "authenticate failed.");
        return false;
    }

    if (res == CR_OK)
    {
        jlog(INFO, "auth: handshake complete.");
        msg[0] = '\0';
        net_send_ok(ss->get_net(), DA_OK, 0, 0, 0, msg);
        /* Ready to handle queries */
        return true;
    }
    else
    {
        net_send_error_packet(ss, 8888, "plugin authenticate hit a bug.", "HY000");
        jlog(ERROR, "auth: handshake uncomplete, res:%d, maybe it's a bug", res);
        return false;
    }
}

/**************************************************************
 * Function:     check_login 
 * Description:   
 * Author: 
 * Date:          
 * Input: 
 * Output:  
 * Return:   @retval true  success,@retval false  error
 * Other:
 **************************************************************/
bool
check_login(class session *ss)
{
    TC_NET *net = NULL;
    char ip[NI_MAXHOST];
    /* remote (peer) port */
    uint16 peer_port;

    net = (TC_NET *) malloc(sizeof (TC_NET));
    if (NULL == net)
    {
        jlog(ERROR, "net init failed.");
        return false;
    }
    memset(net, 0, sizeof (TC_NET));
    ss->set_net(net);

    net_init(ss->get_net(), &ss->sd);

    if (false == get_client_addr(net, ip, &peer_port, NI_MAXHOST))
    {
        jlog(ERROR, "get client addr error, hit a bug");
        return false;
    }

    if (false == login_acl_check_host(ip))
    {
        jlog(WARNING, "%s not allowd login", ip);
        net_send_error_packet(ss, 1130 /*ER_HOST_NOT_PRIVILEGED*/,
                " not allowed access.", ip);
        return false;
    }

    ss->set_host(ip);
    return acl_authenticate(ss);
}
