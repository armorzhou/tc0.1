/* 
 * File:   user_acl.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 上午11:52
 */

#ifndef USER_ACL_H
#define	USER_ACL_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif

#ifndef ACL_DB_H
#include "acl_db.h"
#endif
    
    typedef struct st_user_acl {
        char user[USER_LENGTH];
        char host[HOST_LENGTH];
        char password[PASSWORD_LENGTH];
        char salt[SALT_LENGTH];
        bool super;
        bool global;
        ulong access;
        acl_db *resource;
        struct st_user_acl *next;
    } user_acl;


#ifdef	__cplusplus
}
#endif

#endif	/* USER_ACL_H */

