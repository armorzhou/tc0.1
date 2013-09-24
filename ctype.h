/* 
 * File:   ctype.h
 * Author: tangchao
 *
 * Created on 2013年9月24日, 上午11:05
 */

#ifndef CTYPE_H
#define	CTYPE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#ifndef	_SYS_TYPES_H
#include <sys/types.h>
#endif
    
    typedef char bool;
    
    typedef unsigned char uchar; /* Short for unsigned char */
    typedef unsigned long long int ulonglong; /* ulong or unsigned long long */
    typedef long long int longlong;
    typedef signed char int8; /* Signed integer >= 8  bits */
    typedef unsigned char uint8; /* Unsigned integer >= 8  bits */
    typedef short int16;
    typedef unsigned short uint16;
    typedef int int32;
    typedef unsigned int uint32;
    typedef longlong int64;
    typedef ulonglong uint64;

#ifdef	__cplusplus
}
#endif

#endif	/* CTYPE_H */

