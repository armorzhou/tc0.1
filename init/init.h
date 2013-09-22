/* 
 * File:   init.h
 * Author: tangchao
 *
 * Created on 2013年9月22日, 下午1:39
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C"
{
#endif

void first_init();
void set_port(unsigned int *port);
void set_mport(unsigned int *port);
void set_servername(char *servername);
void set_version();
void set_basedir(char *dir);
void set_datadir(char *dir);
void set_backupdir(char *dir);

#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

