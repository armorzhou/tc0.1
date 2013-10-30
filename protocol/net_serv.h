/* 
 * File:   net_serv.h
 * Author: tangchao
 *
 * Created on 2013年10月25日, 下午1:05
 */

#ifndef NET_SERV_H
#define	NET_SERV_H
#include <sys/types.h>

class NET_SERV
{
public:

    NET_SERV(){}
    NET_SERV(int sock, int io_mode) : sd(sock), fcntl_mode(io_mode)
    {
    }

    NET_SERV(const NET_SERV& orig)
    {
    }

    virtual ~NET_SERV()
    {
    }

    void set_net(int sock, int mode)
    {
        this->sd = sock;
        this->fcntl_mode = mode;
        this->retry_count = 10;
    }
    
    bool net_is_blocking();
    bool net_should_retry(uint *retry_count);
    size_t net_write(const unsigned char* buf, size_t size);
    size_t net_read(unsigned char* buf, size_t size);
private:
    int sd;
    int fcntl_mode; /* Buffered fcntl(sd,F_GETFL) */
    uint retry_count; //   
};

#endif	/* NET_SERV_H */

