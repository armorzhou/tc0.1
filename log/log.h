/* 
 * File:   log.h
 * Author: tangchao
 *
 * Created on 2013年5月13日, 上午11:31
 */

#ifndef LOG_H
#define	LOG_H

#include <stdlib.h>
#include <string.h>

void dbug(const char *fmt, ...);
void dbug_error(const char *fmt, ...);
void dbug_warning(const char *fmt, ...);
void dbug_info(const char *fmt, ...);
void dbug_print(const char *fmt, ...);

/* 统一日志函数 */
#define TCLOG(a, fmt, args...)  \
        do {    \
        if (!strcmp(a, "debug"))        \
                dbug(fmt, ##args);      \
        else if (!strcmp(a, "error"))    \
              dbug_error(fmt, ##args);  \
        else if (!strcmp(a, "warning")) \
                dbug_warning(fmt, ##args);      \
        else if (!strcmp(a, "info"))    \
                dbug_info(fmt, ##args); \
        else if (!strcmp(a, "print"))   \
                dbug_print(fmt, ##args);        \
        } while (0)


#endif	/* LOG_H */

