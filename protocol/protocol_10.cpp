/* 
 * File:   PROTOCOL_10.cpp
 * Author: tangchao
 * 
 * Created on 2013年10月23日, 下午2:15
 */

#include <iostream>
#include <sys/errno.h>
#include <fcntl.h>
#include "../include/byte_order_genric.h"
#include "protocol_10.h"

using namespace std;
#define IO_SIZE			4096

int PROTOCOL_10::get_socket()
{
    return this->sd;
}

unsigned char* PROTOCOL_10::get_buffer()
{
    return this->buff;
}

unsigned char *PROTOCOL_10::get_read_pos()
{
    return this->read_pos;
}

void PROTOCOL_10::read_packets_plus()
{
    this->packets_read++;
}

void PROTOCOL_10::write_packets_plus()
{
    this->packets_write++;
}

ulong PROTOCOL_10::get_read_packets()
{
    return this->packets_read;
}

ulong PROTOCOL_10::get_write_packets()
{
    return this->packets_read;
}

bool
PROTOCOL_10::init(int socket, int io_mode)
{
    net.set_net(socket, io_mode);
    this->sd = socket;
    max_packet_size = MAX_PACKET_LENGTH;
    buff = (unsigned char*) malloc((size_t) max_packet_size +
            NET_HEADER_SIZE + COMP_HEADER_SIZE);
    if (NULL == buff)
    {
        cout << "malloc buff error.";
        return false;
    }
    memset(buff, 0, max_packet_size + NET_HEADER_SIZE + COMP_HEADER_SIZE);
    buff_end = buff + max_packet_size;
    write_pos = read_pos = buff;
    pkt_nr = 0;
    packets_read = packets_write = 0;
    where_b = 0;
    status = WAITING;
    return true;
}

void
PROTOCOL_10::clean()
{
    pkt_nr = 0;
    where_b = 0;
    status = WAITING;
}

bool
PROTOCOL_10::net_realloc(size_t length)
{
    unsigned char *buff;
    size_t pkt_length;

    if (length >= this->max_packet_size)
    {
        cout << "Packet too large. Max size: " << this->max_packet_size;
        return false;
    }
    pkt_length = (length + IO_SIZE - 1) & ~(IO_SIZE - 1);
    /*
      We must allocate some extra bytes for the end 0 and to be able to
      read big compressed blocks + 1 safety byte since uint3korr() in
      net_read_packet() may actually read 4 bytes depending on build flags and
      platform.
     */
    if (!(buff = (unsigned char*) realloc((char*) this->buff, pkt_length +
            NET_HEADER_SIZE + COMP_HEADER_SIZE + 1)))
    {
        cout << "realloc net buffer error.";
        return false;
    }
    this->buff = this->write_pos = buff;
    this->buff_end = buff + (this->max_packet_size = (ulong) pkt_length);
    return 0;
}

/**************************************************************
  Function:net_store_length
  Description:  Store an integer with simple packing into a output package
  Author: 
  Date:         
  Output:     pkg			Store the packed integer here
    length		integers to store
  Return:   Position in 'pkg' after the packed length
  Other:    This is mostly used to store lengths of strings.
    We have to cast the result for the LL() becasue of a bug in Forte CC compiler.
 **************************************************************/
unsigned char *
PROTOCOL_10::net_store_length(unsigned char *packet, unsigned long long length)
{
    if (length < (unsigned long long) LL(251))
    {
        *packet = (unsigned char) length;
        return packet + 1;
    }
    /* 251 is reserved for NULL */
    if (length < (unsigned long long) LL(65536))
    {
        *packet++ = 252;
        int2store(packet, (uint) length);
        return packet + 2;
    }
    if (length < (unsigned long long) LL(16777216))
    {
        *packet++ = 253;
        int3store(packet, (ulong) length);
        return packet + 3;
    }
    *packet++ = 254;
    int8store(packet, length);
    return packet + 8;
}

/**************************************************************
  Function:net_store_data
  Description:for send ok message
  Author: 
  Date:         
  Output: 
  Return:
  Other:
 **************************************************************/
unsigned char *
PROTOCOL_10::net_store_data(unsigned char *to, const unsigned char *from, size_t length)
{
    to = net_store_length(to, length);
    memcpy(to, from, length);
    return to + length;
}

size_t
PROTOCOL_10::net_read_raw_loop(size_t count)
{
    bool eof = true;
    unsigned int retry_count = 0;
    unsigned char *buf = buff + where_b;

    while (count)
    {
        size_t recvcnt = net.net_read(buf, count);

        /* socket error (-1) indicates an error. */
        if (recvcnt == (size_t) - 1)
        {
            /* A recoverable I/O error occurred? */
            if (net.net_should_retry(&retry_count))
            {
                continue;
            }
            else
            {
                break;
            }
        }
            /* Zero indicates end of file. */
        else if (0 == recvcnt)
        {
            eof = false;
            break;
        }
        count -= recvcnt;
        buf += recvcnt;
    }

    return count;
}

bool
PROTOCOL_10::net_read_packet_header()
{
    unsigned char nr;
    size_t count = NET_HEADER_SIZE;

    if (!net_read_raw_loop(count))
    {
        return false;
    }

    nr = buff[this->where_b + 3];
    /*
      Verify packet serial number against the truncated packet counter.
      The local packet counter must be truncated since its not reset.
     */
    if (nr != (unsigned char) this->pkt_nr)
    {
        /* Not a TC_NET error on the client. XXX: why? */
        cout << "packet serial number error";
        return false;
    }

    this->pkt_nr++;
    return true;
}

ulong
PROTOCOL_10::net_read_packet()
{
    size_t pkt_len, pkt_data_len;
    this->status = RD;

    /* Retrieve packet length and number. */
    if (false == net_read_packet_header())
    {
        goto error;
    }

    /* The length of the packet that follows. */
    pkt_len = uint3korr(this->buff + this->where_b);

    /* End of big multi-packet. */
    if (!pkt_len)
    {
        goto end;
    }

    pkt_data_len = pkt_len + this->where_b;

    /* Expand packet buffer if necessary. */
    if ((pkt_data_len >= this->max_packet_size) && net_realloc(pkt_data_len))
    {
        goto error;
    }

    /* Read the packet data (payload). */
    if (!net_read_raw_loop(pkt_len))
    {
        goto error;
    }

end:
    this->status = WAITING;
    return pkt_len;

error:
    this->status = ERROR;
    return packet_error;
}

ulong
PROTOCOL_10::net_read_data()
{
    size_t len;
    this->clean();

    len = net_read_packet();

    if (len == MAX_PACKET_LENGTH)
    {
        /* First packet of a multi-packet.  Concatenate the packets */
        ulong save_pos = this->where_b;
        size_t total_length = 0;
        do
        {
            this->where_b += len;
            total_length += len;
            len = net_read_packet();
        }
        while (len == MAX_PACKET_LENGTH);
        if (len != packet_error)
            len += total_length;
        this->where_b = save_pos;
    }
    this->read_pos = this->buff + this->where_b;
    if (len != packet_error)
    {
        /* Safeguard for mysql_use_result */
        this->read_pos[len] = 0;
    }

    return len;
}

size_t
PROTOCOL_10::net_write_raw_loop(const unsigned char *buf, size_t count)
{
    unsigned int retry_count = 0;

    while (count)
    {
        size_t sentcnt = net.net_write(buf, count);

        /* VIO_SOCKET_ERROR (-1) indicates an error. */
        if (sentcnt == (size_t) - 1)
        {
            /* A recoverable I/O error occurred? */
            if (net.net_should_retry(&retry_count))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        count -= sentcnt;
        buf += sentcnt;
    }

    return count;
}

bool
PROTOCOL_10::net_write_packet(const unsigned char *packet, size_t length)
{
    this->status = WR;
    if (net_write_raw_loop(packet, length))
    {
        this->status = ERROR;
        return false;
    }
    this->status = WAITING;
    return true;
}

bool
PROTOCOL_10::net_write_buff(const unsigned char *packet, size_t len)
{
    ulong left_length;

    left_length = (ulong) (this->buff_end - this->write_pos);

    if (len > left_length)
    {
        if (this->write_pos != this->buff)
        {
            /* Fill up already used packet and write it */
            memcpy(this->write_pos, packet, left_length);
            if (net_write_packet(this->buff,
                    (size_t) (this->write_pos - this->buff) + left_length))
            {
                return false;
            }
            this->write_pos = this->buff;
            packet += left_length;
            len -= left_length;
        }

        if (len > this->max_packet_size)
        {
            return net_write_packet(packet, len);
        }
        /* Send out rest of the blocks as full sized blocks */
    }
    memcpy(this->write_pos, packet, len);
    this->write_pos += len;
    return true;
}

bool
PROTOCOL_10::net_write_data(const unsigned char *packet, size_t len)
{
    unsigned char buff[NET_HEADER_SIZE];
    this->clean(); /* Safe */

    /*
      Big packets are handled by splitting them in packets of MAX_PACKET_LENGTH
      length. The last packet is always a packet that is < MAX_PACKET_LENGTH.
      (The last packet may even have a length of 0)
     */
    while (len >= MAX_PACKET_LENGTH)
    {
        const size_t z_size = MAX_PACKET_LENGTH;
        int3store(buff, z_size);
        buff[3] = (unsigned char) this->pkt_nr++;
        if (net_write_buff(buff, NET_HEADER_SIZE) ||
                net_write_buff(packet, z_size))
        {
            return false;
        }
        packet += z_size;
        len -= z_size;
    }

    /* Write last packet */
    int3store(buff, len);
    buff[3] = (unsigned char) this->pkt_nr++;
    if (!net_write_buff(buff, NET_HEADER_SIZE))
    {
        return false;
    }

    return net_write_buff(packet, len);
}

bool
PROTOCOL_10::net_flush()
{
    bool error = true;

    if (this->buff != this->write_pos)
    {
        error = net_write_packet(this->buff,
                (size_t) (this->write_pos - this->buff));
        this->write_pos = this->buff;
    }

    return (error);
}

/**************************************************************
  Function:net_write_command
  Description:  Send a command to the server.
    The reason for having both header and packet is so that libmysql
    can easy add a header to a special command (like prepared statements)
    without having to re-alloc the string.
    As the command is part of the first data packet, we have to do some data
    juggling to put the command in there, without having to create a new
    packet.
    This function will split big packets into sub-packets if needed.
    (Each sub packet can only be 2^24 bytes)
  Author: 
  Date:         
  @param command	Command in MySQL server (enum enum_server_command)
  @param header	Header to write after command
  @param head_len	Length of header
  @param packet	Query or parameter to query
  @param len		Length of packet
  Output: 
  Return:  @retval
    0	ok
  @retval
    1	error
  Other:
 **************************************************************/
bool
PROTOCOL_10::net_write_command(unsigned char command, const unsigned char *header,
        size_t head_len, const unsigned char *packet, size_t len)
{
    size_t length = len + 1 + head_len; /* 1 extra byte for command */
    unsigned char buff[NET_HEADER_SIZE + 1];
    uint header_size = NET_HEADER_SIZE + 1;

    buff[4] = command; /* For first packet */

    if (length >= MAX_PACKET_LENGTH)
    {
        /* Take into account that we have the command in the first header */
        len = MAX_PACKET_LENGTH - 1 - head_len;
        do
        {
            int3store(buff, MAX_PACKET_LENGTH);
            buff[3] = (unsigned char) this->pkt_nr++;
            if (net_write_buff(buff, header_size) ||
                    net_write_buff(header, head_len) ||
                    net_write_buff(packet, len))
            {
                return (1);
            }
            packet += len;
            length -= MAX_PACKET_LENGTH;
            len = MAX_PACKET_LENGTH;
            head_len = 0;
            header_size = NET_HEADER_SIZE;
        }
        while (length >= MAX_PACKET_LENGTH);
        len = length; /* Data left to be written */
    }
    int3store(buff, length);
    buff[3] = (unsigned char) this->pkt_nr++;

    return test(!net_write_buff(buff, header_size) ||
            (head_len && !net_write_buff(header, head_len)) ||
            !net_write_buff(packet, len) || !net_flush());
}

/**************************************************************
  Function:net_send_error_packet
  Description: 
  Author: 
  Date:         
  Input:  @param ss Thread handler
  @param sql_errno The error code to send
  @param err A pointer to the error message
  Output: 
  Return:   @retval false The message was successfully sent
   @retval TRUE  An error occurred and the messages wasn't sent properly
  Other:
 **************************************************************/
bool
PROTOCOL_10::net_send_error(uint sql_errno, const char *err,
        const char* sqlstate)
{
    uint length;

    /* buff[]: sql_errno:2 + ('#':1 + SQLSTATE_LENGTH:5) + MYSQL_ERRMSG_SIZE:512 */
    char buff[2 + 1 + SQLSTATE_LENGTH + MYSQL_ERRMSG_SIZE], *pos;
    this->clean();

    int2store(buff, sql_errno);
    pos = buff + 2;

    /* The first # is to make the protocol backward compatible */
    buff[2] = '#';
    pos = strmov(buff + 3, sqlstate);

    /* Converted error message is always null-terminated. */
    length = (uint) (strmake(pos, err, MYSQL_ERRMSG_SIZE - 1) - buff);

    return (net_write_command((unsigned char) 255, (unsigned char*) "", 0, (unsigned char*) buff,
            length)) ? false : true;

}

/**************************************************************
  Function:net_send_ok
  Description:   Return ok to the client.
        The ok packet has the following structure:
        - 0               : Marker (1 byte)
        - affected_rows	: Stored in 1-9 bytes
        - id		: Stored in 1-9 bytes
        - server_status	: Copy of 2;  Can be used by client
                to check if we are inside an transaction.
        New in 4.0 protocol
        - warning_count	: Stored in 2 bytes; New in 4.1 protocol
        - message		: Stored as packed length (1-9 bytes) + message.
                Is not stored if no message.
  Author: 
  Date:         
  Input:server_status     The server status
        statement_warn_count  Total number of warnings
        affected_rows	   Number of rows changed by statement
        id		   Auto_increment id for first row (if used)
        message	   Message to send to the client (Used by mysql_status)
        packet	Packet to send
        len		Length of packet
  Output: 
  Return:true The message was successfully sent， false An error occurred 
        and the messages wasn't sent properly false。
  Other:
 **************************************************************/
bool
PROTOCOL_10::net_send_ok(uint server_status, uint statement_warn_count,
        unsigned long long affected_rows, unsigned long long id, const char *message)
{
    unsigned char buf[MYSQL_ERRMSG_SIZE + 10], *pos;

    this->clean();
    buf[0] = 0; // No fields
    pos = net_store_length(buf + 1, affected_rows);
    pos = net_store_length(pos, id);
    int2store(pos, server_status);

    pos += 2;
    /* We can only return up to 65535 warnings in two bytes */
    uint tmp = min(statement_warn_count, 65535U);
    int2store(pos, tmp);
    pos += 2;

    if (message && message[0])
    {
        pos = this->net_store_data(pos, (unsigned char*) message, strlen(message));
    }

    if (this->net_write_data(buf, (size_t) (pos - buf)))
    {
        net_flush();
        return true;
    }
    return false;
}
