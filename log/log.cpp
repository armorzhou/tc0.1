#include <errno.h>
#include <pthread.h>
#include "log.h"

/*0 wirte to file, 1 print stderr, 2 both*/
static int log_output_to_stderr_or_file = 0;
static const char* LogSeverityNames[NUM_SEVERITIES] = {"INFO", "WARNING", "ERROR", "FATAL"};
static char* LogFile;
static int LogFd;
static const int kLogBufSize = 3000;
static int LogLevel = 0;

/**************************************************************
 * Function:get_pid
 * Description:
 * syscall(__NR_gettid) return LWP (thread ID)
 * pthread_self() return posix thread id.
 * Author:tangchao
 * Date: 
 * Input:
 * Output:
 * Return:return thread id
 * Other:
 **************************************************************/
static long int get_pid()
{
    /* Linux kernel ppid */
    return syscall(__NR_gettid);
    /* Posix thread id */
    //return pthread_self();
}

/**************************************************************
 * Function:LogInit
 * Description:
 * Author:tangchao
 * Date: 
 * Input:
 *      @file   log file path
 *      @logLv  log level
 *      @mode   log mode, 0 output to file, 1 to stderr, 2 both.
 * Output:
 * Return:success return 0
 *        failed return 1
 * Other:
 **************************************************************/
int log_init(char *file, int logLv, int mode)
{
    int fd;
    if (!file)
    {
        fprintf(stderr, "open file error, file is null.");
        return 1;
    }
    LogFile = file;
    fd = open(LogFile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);

    if (-1 == fd)
    {
        fprintf(stderr, "open log file %s error.", file);
        return 1;
    }

    LogFd = fd;
    if (logLv < 0 || logLv > LOG_FATAL)
    {
    }
    else
    {
        LogLevel = logLv;
    }

    if (0 <= mode && mode < 3)
    {
        log_output_to_stderr_or_file = mode;
    }

    return 0;
}

/**************************************************************
 * Function:CloseLogServer
 * Description:
 * Author:tangchao
 * Date: 
 * Input:
 * Output:
 * Return:close log fd.
 * Other:
 **************************************************************/
void CloseLogServer()
{
    close(LogFd);
}

/**************************************************************
 * Function:VADoRawLog
 * Description:
 * Author:tangchao
 * Date: 
 * Input:
 * Output:
 * Return:
 * Other:
 **************************************************************/
static int VADoRawLog(char** buf, int* size,
        const char* format, va_list ap)
{
    int n = vsnprintf(*buf, *size, format, ap);
    if (n < 0 || n > *size)
    {
        fprintf(stderr, "found log server bug");
        return 1;
    }
    *size -= n;
    *buf += n;
    return 0;
}

/**************************************************************
 * Function:DoRawLog
 * Description:
 * Author:tangchao
 * Date: 
 * Input:
 * Output:
 * Return:
 * Other:
 **************************************************************/
static int DoRawLog(char** buf, int* size, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int n = vsnprintf(*buf, *size, format, ap);
    va_end(ap);
    if (n < 0 || n > *size)
    {
        fprintf(stderr, "found log server bug");
        return 1;
    }
    *size -= n;
    *buf += n;
    return 0;
}

/**************************************************************
 * Function:RawLog
 * Description:
 * Author:tangchao
 * Date: 
 * Input:
 * Output:
 * Return:close log fd.
 * Other:
 **************************************************************/
void RawLog(LogSeverity severity, const char* file, int line,
        const char* format, ...)
{
    if (severity < LogLevel)
    {
        return;
    }
    struct tm *t;
    time_t lt;
    lt = time(NULL);
    t = localtime(&lt);
    char buffer[kLogBufSize];
    char* buf = buffer;
    int size = sizeof (buffer);

    DoRawLog(&buf, &size, "%c%02d%02d %02d:%02d:%02d %5ld %s:%d]: ",
            LogSeverityNames[severity][0],
            1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,
            get_pid(), file, line);

    va_list ap;
    va_start(ap, format);
    int no_chop = VADoRawLog(&buf, &size, format, ap);
    va_end(ap);
    if (!no_chop)
    {
        DoRawLog(&buf, &size, "\n");
    }
    else
    {
        DoRawLog(&buf, &size, "RAW_LOG ERROR: The Message was too long!\n");
    }

    if (1 == log_output_to_stderr_or_file || 2 == log_output_to_stderr_or_file)
    {
        safe_write(STDERR_FILENO, buffer, strlen(buffer));
    }

    if (0 == log_output_to_stderr_or_file || 2 == log_output_to_stderr_or_file)
    {
        int res = file_write(LogFd, buffer, strlen(buffer));
        if (-1 == res && EBADF == errno)
        {
            CloseLogServer();
            if (!log_init(LogFile, LogLevel, log_output_to_stderr_or_file))
            {
                fprintf(stderr, "log server unknown error, closed.");
            }
        }
    }

    return;
}
