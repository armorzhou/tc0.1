/* 
 * File:   net_serv.cpp
 * Author: tangchao
 * 
 * Created on 2013年10月25日, 下午1:05
 */
#include <iostream>
#include <sys/errno.h>
#include <stddef.h>
#include <fcntl.h>
#include <mysql.h>
#include "../log/jlog.h"
#include "net_serv.h"

using namespace std;
bool
NET_SERV::net_is_blocking()
{
    bool r;
    r = !(this->fcntl_mode & O_NONBLOCK);
    return r;
}
bool
NET_SERV::net_should_retry(uint *retry_count)
{
    bool retry;
    int en = errno;
    en == EINTR || ((!this->net_is_blocking())
                && (en == EAGAIN || en == EWOULDBLOCK));
    retry = en && ((*retry_count)++ < this->retry_count);
    return retry;
}


size_t
NET_SERV::net_read(unsigned char* buf, size_t size)
{
    size_t r;

    jlog(INFO, "socket %d", this->sd);
    if (NULL == buf)
    {
        jlog(WARNING, "invalid pointer.");
        return packet_error;
    }

    r = read(sd, buf, size);
    if (r == (size_t) - 1)
    {
        jlog(WARNING, "net_error Got error %d", errno);;
        return packet_error;
    }

    jlog(INFO, "received  bytes  %lu", r);
    return r;
}

size_t
NET_SERV::net_write(const unsigned char* buf, size_t size)
{
    size_t r;

    r = write(this->sd, buf, size);
    if (r == (size_t) - 1)
    {
        jlog(WARNING, "net_error Got error on write: %d", errno);
    }
    return r;
}
