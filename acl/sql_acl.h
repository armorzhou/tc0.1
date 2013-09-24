/* 
 * File:   sql_acl.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 下午12:01
 */

#ifndef SQL_ACL_H
#define	SQL_ACL_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef USER_ACL_H
#include "user_acl.h"
#endif

    typedef struct st_sql_acl {
        char *user;
        char *host;
        char *passwd;
        char *salt;
        acl_db *resource;
        
    } sql_acl;


#ifdef	__cplusplus
}
#endif

#endif	/* SQL_ACL_H */

