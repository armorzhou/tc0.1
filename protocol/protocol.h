/* 
 * File:   mysql_PROTOCOL.h
 * Author: tangchao
 *
 * Created on 2013年10月23日, 下午2:15
 */

#ifndef PROTOCOLL_H
#define	PROTOCOLL_H
#include <iostream>
#include <string.h>
#include <sys/types.h>

#define MAX_PACKET_LENGTH (256L*256L*256L-1)

using namespace std;

class PROTOCOL
{
public:
    PROTOCOL() {  }
    PROTOCOL(const PROTOCOL& orig) { }
    virtual ~PROTOCOL() {  }
    virtual bool init(int socket, int io_mode) = 0;
    /* read data into buffer */
    virtual ulong net_read_data() = 0;
    /* send to client */
    virtual bool net_write_data(const unsigned char *packet, size_t len) = 0;
    /* send last data */
    virtual bool net_flush() = 0;
    /* send error to client */
    virtual bool net_send_error(uint sql_errno, const char *err,
            const char* sqlstate) = 0;
    /* send ok to client */
    virtual bool net_send_ok(uint server_status, uint statement_warn_count,
            unsigned long long affected_rows, unsigned long long id,
            const char *message) = 0;
    /* return a socket */
    virtual int get_socket() = 0;
    /* return buffer */
    virtual unsigned char* get_buffer() = 0;
        /* return read position */
    virtual unsigned char *get_read_pos() = 0;
    /* read packet counter */
    virtual void read_packets_plus() = 0;
    /* write counter */
    virtual void write_packets_plus() = 0;
    /* return read counter */
    virtual ulong get_read_packets() = 0;
    /* return write counter */
    virtual ulong get_write_packets() = 0;

};

#endif	/* PROTOCOLL_H */
