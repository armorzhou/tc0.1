/* 
 * File:   mysql_native_password_auth.h
 * Author: tangchao
 *
 * Created on 2013年9月26日, 下午6:28
 */

#ifndef MYSQL_NATIVE_PASSWORD_AUTH_H
#define	MYSQL_NATIVE_PASSWORD_AUTH_H

#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif
#ifndef MYSQL_AUTHENTICATE_H
#include "mysql_authenticate.h"
#endif

class mysql_native_password_auth {
public:

    mysql_native_password_auth() {

    }

    mysql_native_password_auth(const mysql_native_password_auth& orig) {

    }

    virtual ~mysql_native_password_auth() {

    }

    virtual void *mysql_acl_authenticate();

    void create_random_str(char *scramble) {
        create_random_string(scramble, SCRAMBLE_LENGTH, myrand);
    }
private:

};

#endif	/* MYSQL_NATIVE_PASSWORD_AUTH_H */

