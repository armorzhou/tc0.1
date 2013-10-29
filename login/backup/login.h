/* 
 * File:   login.h
 * Author: tangchao
 *
 * Created on 2013年9月5日, 下午7:05
 */

#ifndef LOGIN_H
#define	LOGIN_H

#ifndef SESSION_H
#include "../session.h"
#endif
#ifndef SQL_ACL_H
#include "../acl/sql_acl.h"
#endif

bool check_login(class session *session);
int tc_convert(char *outbuf, size_t outlen, char *inbuf, size_t inlen);
int tc_copy_and_convert(char *to, size_t to_length, char *from, size_t from_length);

#endif	/* LOGIN_H */
