/* 
 * File:   privileges.h
 * Author: tangchao
 *
 * Created on 2013年10月17日, 下午6:38
 */

#ifndef PRIVILEGES_H
#define	PRIVILEGES_H

#define NONE_PRI        1
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

#define ALL_PRIVILEGES (NONE_PRI + SELECT_PRI + INSERT_PRI +  UPDATE_PRI + \
                DELETE_PRI + TRUNCATE_PRI + DROP_PRI + CREATE_PRI + GRANT_PRI + \
                REVOKE_PRI +INIT_DB)

#endif	/* PRIVILEGES_H */

