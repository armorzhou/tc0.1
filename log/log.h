/* 
 * File:   log.h
 * Author: tangchao
 *
 * Created on 2013年12月18日, 下午1:19
 */

#ifndef _TCLOG_H
#define	_TCLOG_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include <sys/types.h>    
#include <sys/stat.h>   
#include <fcntl.h>
#include <unistd.h>

#define safe_write(fd, s, len)  syscall(SYS_write, fd, s, len)
#define file_write(fd, s, len)  write(fd, s, len)
#define NUM_SEVERITIES  4

typedef int LogSeverity;
const int LOG_INFO = 0;
const int LOG_WARNING = 1;
const int LOG_ERROR = 2;
const int LOG_FATAL = 3;

#define INFO    LOG_INFO
#define WARNING LOG_WARNING
#define ERROR   LOG_ERROR
#define FATAL   LOG_FATAL

#define  DEBUG_ON 1

void CloseLogServer();
void RawLog(LogSeverity severity, const char* file, int line, const char* format, ...);

#define TCLOG(severity, ...)  \
  do { \
    switch (severity) {  \
      case 3: \
        assert(false);  \
        break;  \
      default: \
         RawLog(severity, __FILE__, __LINE__, __VA_ARGS__); \
    } \
  } while (0)

int log_init(char *file, int logLv, int mode);

#endif	/* LOG_H */
