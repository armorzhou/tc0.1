/* 
 * File:   listen.h
 * Author: tangchao
 *
 * Created on 2013年9月16日, 上午10:24
 */

#ifndef LISTENER_H
#define	LISTENER_H

#define DEFAULT_TCP_PORT        3344
#define DEFAULT_BACK_LOG        1024
#define DEFAULT_TC_PORT_TIMEOUT 10

class LISTENER
{
public:
    void init(int *ip_sock, char *my_bind_addr_str, unsigned int *dport);
};

#endif	/* LISTENER_H */

