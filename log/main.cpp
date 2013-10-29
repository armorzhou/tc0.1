/* 
 * File:   main.cpp
 * Author: tangchao
 *
 * Created on 2013年8月20日, 上午10:15
 */

#include "jlog.h"

using namespace std;
string info_log_file = "/export/log/log_";
string error_log_file = "";
string warning_log_file = "";
string fatal_log_file = "";

int
main(int argc,char** argv)
{
    if(argc)
        cout << "error:" << argv[argc];

#if USE_GOOGLE
    // Initialize Google's logging library.
    jlog_init(info_log_file,error_log_file,warning_log_file,fatal_log_file);
    string a="myshell";
    // ...
    jlog(INFO, "log plugin INFO %d", 100);
    jlog(ERROR, "log plugin ERROR %s", "china");
    jlog(FATAL, "log  plugin  FATAL %s", a.c_str());
#elif USE_LOG4C
    log4c_category_t* mycat = NULL;
    if(log4c_init())
    {
        cout << "log init failed";
        return 1;
    }
    mycat = log4c_category_get("six13log.log.app.test");

    jlog(mycat,LOG4C_PRIORITY_DEBUG,"Debugging app 2");
    jlog(mycat,LOG4C_PRIORITY_ERROR,
            "some error from app1 at line %d in file %s",
            __LINE__,__FILE__);
        jlog(mycat,LOG4C_PRIORITY_ERROR,
            "some error from app1 at line %d in file %s",
            __LINE__,__FILE__);
            jlog(mycat,LOG4C_PRIORITY_ERROR,
            "some error from app1 at line %d in file %s",
            __LINE__,__FILE__);
#endif
    return 0;
}
