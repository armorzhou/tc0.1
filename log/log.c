#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>

void dbug(const char *fmt, ...)
{
    char timestr[50];
    time_t time_tmp;
    struct tm *nowtime;
    time(&time_tmp);
    nowtime = localtime(&time_tmp);
    strftime(timestr, 50, "%Y-%m-%d %H:%M:%S", nowtime);
    printf("%s\t", timestr);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    return;
}

void dbug_error(const char *fmt, ...)
{
    char timestr[50];
    time_t time_tmp;
    struct tm *nowtime;
    time(&time_tmp);
    nowtime = localtime(&time_tmp);
    strftime(timestr, 50, "%Y-%m-%d %H:%M:%S", nowtime);
    printf("%s\t", timestr);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    return;
}

void dbug_warning(const char *fmt, ...)
{
    char timestr[50];
    time_t time_tmp;
    struct tm *nowtime;
    time(&time_tmp);
    nowtime = localtime(&time_tmp);
    strftime(timestr, 50, "%Y-%m-%d %H:%M:%S", nowtime);
    printf("%s\t", timestr);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    return;
}

void dbug_info(const char *fmt, ...)
{
    char timestr[50];
    time_t time_tmp;
    struct tm *nowtime;
    time(&time_tmp);
    nowtime = localtime(&time_tmp);
    strftime(timestr, 50, "%Y-%m-%d %H:%M:%S", nowtime);
    printf("%s\t", timestr);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    return;
}

void dbug_print(const char *fmt, ...)
{
    char timestr[50];
    time_t time_tmp;
    struct tm *nowtime;
    time(&time_tmp);
    nowtime = localtime(&time_tmp);
    strftime(timestr, 50, "%Y-%m-%d %H:%M:%S", nowtime);
    printf("%s\t", timestr);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    return;
}
