/* 
 * File:   acl_db.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 上午11:52
 */

#ifndef ACL_DB_H
#define	ACL_DB_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif
#ifndef ACL_TABLE_H
#include "acl_table.h"
#endif

    
    typedef struct st_acl_db {
        char db_name[DB_LENGTH];
        bool global;
        ulong access;
        acl_table *tables;
        struct st_acl_db *next;
    } acl_db;
    
#ifdef	__cplusplus
}
#endif

#endif	/* ACL_DB_H */

