/* 
 * File:   login.h
 * Author: tangchao
 *
 * Created on 2013年10月24日, 下午3:19
 */

#ifndef LOGIN_H
#define	LOGIN_H

#include "../protocol/protocol_10.h"
#include "server_connection_cnf.h"

class LOGIN
{
public:
    LOGIN(){}
    LOGIN(const LOGIN& orig){}
    virtual ~LOGIN(){}
    
    virtual void init(SERVER_CONNECTION_CNF* scc, PROTOCOL* pl) = 0;
    virtual bool check_login() = 0;
private:

};

#endif	/* LOGIN_H */

