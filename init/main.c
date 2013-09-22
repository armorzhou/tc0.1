/* 
 * File:   main.c
 * Author: tangchao
 *
 * Created on 2013年9月22日, 下午2:23
 */

#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "../gvars.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    char *dir = "/export/servers/data/mysql/dba/manager/dbfile/ng.log";
    unsigned int port = 1111, mport = 2222;
    
    first_init();

    set_port(&port);
    set_mport(&mport);
    set_servername(NULL);
    set_version();
    set_basedir(dir);
    set_datadir(dir);
    set_backupdir(dir);

    printf("servername=%s\n", server_name);
    printf("version=%s\n", server_version);
    printf("tc_port=%u\n", tc_port);
    printf("mport=%u\n", manage_port);
    printf("basedir=%s\n", basedir);
    printf("datadir=%s\n", datadir);
    printf("backupdir=%s\n", backupdir);
    
    return (EXIT_SUCCESS);
}

