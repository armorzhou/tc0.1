/* 
 * File:   gdef.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 上午11:03
 */

#ifndef GDEF_H
#define	GDEF_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef CTYPE_H
#include "ctype.h"
#endif 
#ifndef true
#define true 1
#define false 0
#endif
    
#define USER_LENGTH         32
#define HOST_LENGTH             32
#define PASSWORD_LENGTH         64
#define SALT_LENGTH             64
    
#define DB_LENGTH       32
#define TABLE_LENGTH    32
    
#define NONE_PRI        1       //特殊标志位，表示无任何权限的用户
#define SELECT_PRI      2
#define INSERT_PRI      4
#define UPDATE_PRI      8
#define DELETE_PRI      16
#define TRUNCATE_PRI    32
#define DROP_PRI        64
#define CREATE_PRI      128
#define GRANT_PRI       256
#define REVOKE_PRI      512
#define INIT_DB         1024

#define ALL_PRIVILEGES (NONE_PRI + SELECT_PRI + INSERT_PRI +  UPDATE_PRI \
        + DELETE_PRI + TRUNCATE_PRI + DROP_PRI + CREATE_PRI + GRANT_PRI \
        + REVOKE_PRI +INIT_DB)
    


#ifdef	__cplusplus
}
#endif

#endif	/* GDEF_H */
