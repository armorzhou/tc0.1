#ifndef PROTOCOL_10_H
#define	PROTOCOL_10_H

#include "../include/base.h"
#include "net_serv.h"
#include "protocol.h"

using namespace std;

class PROTOCOL_10 : public PROTOCOL, public mybase
{
public:
    PROTOCOL_10(){}
    PROTOCOL_10(const PROTOCOL_10& orig) {}
    virtual ~PROTOCOL_10() {}
    bool init(int socket, int io_mode);
    ulong net_read_data();
    bool net_write_data(const unsigned char *packet, size_t len);
    /* send last data */
    bool net_flush();
    bool net_send_error(uint sql_errno, const char *err,
            const char* sqlstate);
    bool net_send_ok(uint server_status, uint statement_warn_count,
            unsigned long long affected_rows, unsigned long long id,
            const char *message);
    int get_socket();
    unsigned char* get_buffer();
    /* return read position */
    unsigned char *get_read_pos();
    /* read packet counter */
    void read_packets_plus();
    /* write counter */
    void write_packets_plus();
    /* return read counter */
    ulong get_read_packets();
    /* return write counter */
    ulong get_write_packets();

    bool net_realloc(size_t length);
    unsigned char *net_store_length(unsigned char *packet, unsigned long long length);
    unsigned char *net_store_data(unsigned char *to, const unsigned char *from, size_t length);

    size_t net_read_raw_loop(size_t count);
    bool net_read_packet_header();
    ulong net_read_packet();

    size_t net_write_raw_loop(const unsigned char *buf, size_t count);
    bool net_write_packet(const unsigned char *packet, size_t length);
    bool net_write_buff(const unsigned char *packet, size_t len);

    bool net_write_command(unsigned char command, const unsigned char *header,
            size_t head_len, const unsigned char *packet, size_t len);

private:
    ulong pkt_nr;
    ulong max_packet_size;
    ulong packet_sequence_number; // send data
    ulong packets_read; // read data
    ulong packets_write; // write data
    unsigned char* buff;
    unsigned char* write_pos, *read_pos;
    unsigned char* buff_end;
    /* */
    ulong where_b;
    int sd; //socket
    
    enum
    {
        RD, WR, WAITING, ERROR
    } status; // read, write
    NET_SERV net;
};

#endif	/* PROTOCOL_10_H */
