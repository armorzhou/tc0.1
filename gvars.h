/* 
 * File:   gvars.h
 * Author: tangchao
 * 新增全局变量到init/variables.h中，然后在这里做声明。
 * Created on 2013年9月22日, 下午2:34
 */

#ifndef GVARS_H
#define	GVARS_H

#ifdef	__cplusplus
extern "C"
{
#endif

    extern int ip_sock;
    extern unsigned int tc_port;
    extern unsigned int manage_port;

    extern char server_name[32];
    extern char server_version[16];

    extern char logfile[255];

    extern char basedir[255];
    extern char datadir[255];
    extern char backupdir[255];


#ifdef	__cplusplus
}
#endif

#endif	/* GVARS_H */

