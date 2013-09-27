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

