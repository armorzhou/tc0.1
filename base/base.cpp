#include <stdio.h>
#include <stdarg.h>
#include "base.h"
#include "../ctype.h"
#include "../tc_global.h"

/**************************************************************
  Function:     strmake
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
char *strmake(register char *dst, register const char *src, size_t length)
{
  while (length--)
    if (! (*dst++ = *src++))
      return dst-1;
  *dst=0;
  return dst;
}

/**************************************************************
  Function:     strnmov
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
char *strnmov(register char *dst, register const char *src, size_t n)
{
  while (n-- != 0) {
    if (!(*dst++ = *src++)) {
      return (char*) dst-1;
    }
  }
  return dst;
}

/**************************************************************
  Function:     strxmov
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
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

