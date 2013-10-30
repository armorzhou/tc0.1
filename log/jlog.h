/* 
 * File:   log.h
 * Author: tangchao
 *
 * Created on 2013年8月20日, 上午11:24
 */

#ifndef JLOG_H
#define	JLOG_H

#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif

#define USE_GOOGLE USE_GOOGLE_LOG_
#define USE_LOG4C USE_LOG4C_LOG_

#define JOY_THREAD_SAFE_ LOG_FOR_QINBO_  //use thread safe log

#if USE_GOOGLE
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>
#include <glog/logging.h>
#include <glog/raw_logging.h>

using namespace std;

void jlog_init(string info_log_file, string error_log_file, 
        string warning_log_file, string fatal_log_file);

#if JOY_THREAD_SAFE_
#define JLOG    jlog
#define jlog(severity, ...) \
  do { \
    switch (google::GLOG_ ## severity) {  \
      case 0: \
        RAW_LOG_INFO(__VA_ARGS__); \
        break; \
      case 1: \
        RAW_LOG_WARNING(__VA_ARGS__); \
        break; \
      case 2: \
        RAW_LOG_ERROR(__VA_ARGS__); \
        break; \
      case 3: \
        RAW_LOG_FATAL(__VA_ARGS__); \
        break; \
      default: \
        break; \
    } \
  } while (0)
#else
#define jlog(A)                 LOG(A)
#endif

#define djlog(B)                DLOG(B)
#define djlog_if(C, D)          DLOG_IF(C, D)
#define djlog_every_n(E, F)     DLOG_EVERY_N(E, F)

#elif USE_LOG4C
extern "C"
{
#include <log4c/version.h>
#include <log4c/init.h>
#include <log4c/rc.h>
#include <log4c/appender.h>
#include <log4c/rollingpolicy.h>
#include <log4c/category.h>
#include <log4c/layout.h>
#include <log4c/logging_event.h>
#include <log4c/priority.h>

#define jlog log4c_category_log

    /** fatal */
#define FATAL LOG4C_PRIORITY_FATAL
    /** alert */
#define ALTERT LOG4C_PRIORITY_ALERT
    /** crit */
#define CRIT LOG4C_PRIORITY_CRIT	
    /** error */
#define ERROR LOG4C_PRIORITY_ERROR	
    /** warn */
#define WARNING LOG4C_PRIORITY_WARN	
    /** notice */
#define NOTICE LOG4C_PRIORITY_NOTICE	
    /** info */
#define INFO LOG4C_PRIORITY_INFO	
    /** debug */
#define DEBUG LOG4C_PRIORITY_DEBUG	
    /** trace */
#define TRACE LOG4C_PRIORITY_TRACE	
    /** notset */
#define NOTSET LOG4C_PRIORITY_NOTSET	
    /** unknown */
#define UNKNOWN LOG4C_PRIORITY_UNKNOWN
}
#endif

#endif	/* JLOG_H */
