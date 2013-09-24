/* 
 * File:   acl_table.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 上午11:53
 */

#ifndef ACL_TABLE_H
#define	ACL_TABLE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif
    
    /* acl */
    typedef struct st_acl_table {
        char table_name[TABLE_LENGTH];
        ulong access;
        struct st_acl_table *next;
    }acl_table;
    

#ifdef	__cplusplus
}
#endif

#endif	/* ACL_TABLE_H */

