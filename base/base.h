/* 
 * File:   base.h
 * Author: tangchao
 *
 * Created on 2013年8月29日, 下午3:31
 */

#ifndef BASE_H
#define	BASE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>

#define NullS		(char *) 0
#define strmov(A,B) __builtin_stpcpy((A),(B))
#define test(a)		((a) ? 1 : 0)

class mybase
{
public:

    char *strmake(register char *dst, register const char *src, size_t length)
    {
        while (length--)
            if (!(*dst++ = *src++))
                return dst - 1;
        *dst = 0;
        return dst;
    }

    char *strnmov(register char *dst, register const char *src, size_t n)
    {
        while (n-- != 0)
        {
            if (!(*dst++ = *src++))
            {
                return (char*) dst - 1;
            }
        }
        return dst;
    }

    char *
    strxmov(char *dst, const char *src, ...)
    {
        va_list pvar;

        va_start(pvar, src);
        while (src != NullS)
        {
            while ((*dst++ = *src++));
            dst--;
            src = va_arg(pvar, char *);
        }
        va_end(pvar);
        *dst = 0; /* there might have been no sources! */
        return dst;
    }
};

#endif	/* BASE_H */

