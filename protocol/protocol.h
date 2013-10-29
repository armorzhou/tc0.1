/* 
 * File:   mysql_PROTOCOL.h
 * Author: tangchao
 *
 * Created on 2013年10月23日, 下午2:15
 */

#ifndef PROTOCOLL_H
#define	PROTOCOLL_H
#define MAX_PACKET_LENGTH (256L*256L*256L-1)

class PROTOCOL
{
public:
    virtual ~PROTOCOL() { }

    /* read data into buffer */
    virtual ulong net_read_data();
    /* send data to client */
    virtual bool net_write_data(const unsigned char *, size_t );
    /* send last data */
    virtual bool net_flush();
    /* send error */
    virtual bool net_send_error(uint , const char *, const char* );
    /* send ok packet */
    virtual bool net_send_ok(uint, uint, unsigned long long,
            unsigned long long, const char *);
    virtual int get_socket();
    virtual unsigned char* get_buffer();
};

#endif	/* PROTOCOLL_H */
