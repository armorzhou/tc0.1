/* 
 * File:   variables.h
 * Author: tangchao
 * 
 * Created on 2013年9月22日, 下午1:38
 */

/* 需要初始化的全局变量全部放在这里 */

#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C"
{
#endif

#define DEFAULT_TC_PORT         3344
#define DEFAULT_MANAGE_PORT     5566
#define DEFAULT_SERVER_NAME     "TC"
#define DEFAULT_SERVER_NAME_LENGTH      2
#define VERSION_HEAD    "0"
#define VERSION_MIDDLE  "0"
#define VERSION_SMALL   "0"

    unsigned int tc_port;
    unsigned int manage_port;

    char server_name[32];
    char server_version[16];

    char logfile[255];

    char basedir[255];
    char datadir[255];
    char backupdir[255];

#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

