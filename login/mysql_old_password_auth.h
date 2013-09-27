/* 
 * File:   mysql_old_password_auth.h
 * Author: tangchao
 *
 * Created on 2013年9月26日, 下午6:29
 */

#ifndef MYSQL_OLD_PASSWORD_AUTH_H
#define	MYSQL_OLD_PASSWORD_AUTH_H

#ifndef MYSQL_AUTHENTICATE_H
#include "mysql_authenticate.h"
#endif

class mysql_old_password_auth {
public:
    mysql_old_password_auth(){
        
    }
    mysql_old_password_auth(const mysql_old_password_auth& orig){
        
    }
    virtual ~mysql_old_password_auth(){
        
    }
    
    virtual void *mysql_acl_authenticate();
private:

};

#endif	/* MYSQL_OLD_PASSWORD_AUTH_H */

