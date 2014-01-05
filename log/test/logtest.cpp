/* 
 * File:   logtest.cpp
 * Author: tangchao
 *
 * Created on 2014年1月5日, 上午11:36
 */

#include <cstdlib>
#include "../log.h"

using namespace std;
/*
 * 
 */
int main(int argc, char** argv)
{
    log_init("/export/log/tc.log", 0, 2);
    TCLOG(INFO, "log server start");
    TCLOG(INFO, "log server version %d", 100);
    TCLOG(WARNING, "log warning, %s", "log log gog go");
    TCLOG(ERROR, "log error, error number %d", 123);
    TCLOG(FATAL, "log fatal");
    return 0;
}

