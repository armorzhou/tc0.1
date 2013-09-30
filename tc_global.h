/* 
 * File:   tc_global.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 上午11:04
 */

#ifndef TC_GLOBAL_H
#define	TC_GLOBAL_H

#ifndef _mysql_h
#include <mysql.h>
#endif

#define INVALID_ACL     2
#define ACL_INSERT      2 << 1
#define ACL_DELETE      2 << 2
#define ACL_UPDATE      2 << 3
#define ACL_SELECT      2 << 4
#define ACL_ALTER       2 << 5
#define ACL_CREATE      2 << 6
#define ACL_DROP        2 << 7
#define ACL_TRUNCATE    2 << 8
#define ACL_CONNECT     2 << 9
#define ACL_CONNECT_DB  2 << 10

#define ACL_ALL         (ACL_INSERT + ACL_DELETE + ACL_UPDATE + ACL_SELECT + \
                        ACL_ALTER + ACL_CREATE + ACL_DROP + ACL_TRUNCATE + \
                        ACL_CONNECT + ACL_CONNECT_DB)

typedef struct st_session session;
struct st_session {
    ulong client_capabilities;
    NET net;
};

extern int ip_sock;
extern unsigned int tc_port;
extern unsigned int manage_port;
extern char server_name[32];
extern char server_version[16];
extern char logfile[255];
extern char basedir[255];
extern char datadir[255];
extern char backupdir[255];

extern unsigned int protocol_version;
extern char server_version[SERVER_VERSION_LENGTH];

extern char empty_c_string[1]; /* used for not defined db */
#include <time.h>
extern time_t server_start_time;
extern struct rand_struct *myrand;

#endif	/* TC_GLOBAL_H */

