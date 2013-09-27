#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mysql.h>
#include "variables.h"

static struct rand_struct MYSCRAMBLERANDST;
struct rand_struct *myrand = &MYSCRAMBLERANDST;

void first_init()
{
    tc_port = 0;
    manage_port = 0;
    
    memset(server_name, 0, 32);
    server_name[0] = '\0';
    
    memset(server_version, 0, 16);
    server_version[0] = '\0';

    memset(logfile, 0, 255);
    logfile[0] = '\0';
    
    memset(basedir, 0, 255);
    basedir[0] = '\0';
    
    memset(datadir, 0, 255);
    datadir[0] = '\0';
    
    memset(backupdir, 0, 255);
    backupdir[0] = '\0';

    return;
}

void set_port(unsigned int *port)
{
    if(port)
        tc_port = *port;
    else
        tc_port = DEFAULT_TC_PORT;
    return;
}

void set_mport(unsigned int *port)
{
    if(port)
        manage_port = *port;
    else
        manage_port = DEFAULT_MANAGE_PORT;
    return;
}

void set_servername(char *servername)
{
    size_t server_name_length;

    if (!servername)
    {
        memcpy(server_name, DEFAULT_SERVER_NAME, DEFAULT_SERVER_NAME_LENGTH);
        return;
    }

    server_name_length = strlen(servername);
    if (server_name_length > 32)
    {
        memcpy(server_name, DEFAULT_SERVER_NAME, DEFAULT_SERVER_NAME_LENGTH);
        fprintf(stderr, "[BUG FOUND]:server_name_length greater than 32\n");
        exit(1);
        return;
    }

    memcpy(server_name, servername, server_name_length);

    return;
}

void set_version()
{
    unsigned int version_length = 0, i, length;
    char *vtmp;
    char *vhead = VERSION_HEAD;
    char *vmiddle = VERSION_MIDDLE;
    char *vsmall = VERSION_SMALL;
    
    version_length = strlen(vhead) + strlen(vmiddle) + strlen(vsmall) + 2;
    
    if(version_length > 16)
    {
        fprintf(stderr, "[BUG FOUND]:version_length greater than 32.\n");
        exit(1);
        return;
    }

    vtmp = (char *) malloc(version_length);
    if (!vtmp)
    {
        fprintf(stderr, "no enougth memeory.\n");
        exit(1);
        return;
    }

    i = 0;
    length = 0;
    while (vhead[i] != '\0')
    {
        vtmp[i] = vhead[i];
        i++;
        length++;
    }

    vtmp[i] = '.';
    length++;

    i = 0;
    while (vmiddle[i] != '\0')
    {
        vtmp[length] = vmiddle[i];
        i++;
        length++;
    }

    vtmp[i] = '.';
    length++;
    i = 0;
    while (vsmall[i] != '\0')
    {
        vtmp[length] = vsmall[i];
        i++;
        length++;
    }
    vtmp[length] = '\0';
    memcpy(server_version, vtmp, length);
    free(vtmp);
    
    return;
}

void set_basedir(char *dir)
{
    unsigned int dirlen;
    
    if (!dir)
    {
        basedir[0] = '/';
        basedir[1] = '\0';
        return;
    }

    dirlen = strlen(dir);
    if(dirlen > 255)
    {
        fprintf(stderr, "[BUG FOUND]:dirlen is greater than 255\n");
        exit(1);
    }

    memcpy(basedir, dir, dirlen);
    
    return;
}

void set_datadir(char *dir)
{
    unsigned int dirlen;
    if (!dir)
    {
        fprintf(stderr, "[BUG FOUND]:datadir unspecified.\n");
        exit(1);
    }

    dirlen = strlen(dir);
    if(dirlen > 255)
    {
        fprintf(stderr, "[BUG FOUND]:dirlen is greater than 255\n");
        exit(1);
    }

    memcpy(datadir, dir, dirlen);
    
    return;
}

void set_backupdir(char *dir)
{
    unsigned int dirlen;
    if (!dir)
    {
        fprintf(stderr, "[BUG FOUND]:datadir unspecified.\n");
        exit(1);
    }

    dirlen = strlen(dir);
    if(dirlen > 255)
    {
        fprintf(stderr, "[BUG FOUND]:dirlen is greater than 255\n");
        exit(1);
    }

    memcpy(backupdir, dir, dirlen);
    
    return;
}
