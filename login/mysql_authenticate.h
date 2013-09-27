/* 
 * File:   mysql_authenticate.h
 * Author: tangchao
 *
 * Created on 2013年9月26日, 下午6:31
 */

#ifndef MYSQL_AUTHENTICATE_H
#define	MYSQL_AUTHENTICATE_H

class mysql_authenticate {
public:
    mysql_authenticate();
    mysql_authenticate(const mysql_authenticate& orig);
    virtual ~mysql_authenticate();
    
    virtual void *mysql_acl_authenticate() = 0;
private:

};

#endif	/* MYSQL_AUTHENTICATE_H */

