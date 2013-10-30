#include "jlog.h"

/**************************************************************
  Function:     
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
#if USE_GOOGLE
void
jlog_init(string info_log_file, string error_log_file, string warning_log_file,
        string fatal_log_file)
{
    FLAGS_minloglevel = 1;
    FLAGS_log_dir = "/export/log";
    google::InitGoogleLogging(PRODUCT_NAME);
    google::SetLogDestination(google::INFO, info_log_file.c_str()); //大于INFO级别的，记录在当前目录，文件以为mylog_前缀
    google::SetLogDestination(google::ERROR, error_log_file.c_str()); //不为ERROR建立日志文件
    google::SetLogDestination(google::WARNING, warning_log_file.c_str()); //不为ERROR建立日志文件
    google::SetLogDestination(google::FATAL, fatal_log_file.c_str());
}
#endif
