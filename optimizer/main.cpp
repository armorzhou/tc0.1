#include "statistics.h"
#include "../log/jlog.h"
#include "cs_db.h"
#include "cs_field.h"
#include "cs_table.h"

/*
 * 输入值：
 * 函数功能：测试代码，测试用
 * 返回值：
 */
int
cbo_test()
{
    class statistics *stic = NULL;   
    class mysql_meta *mysql_conf = NULL;
    mysql_conf = new mysql_meta("172.17.4.50","3333","dba",
            "yvhkfhvk","joy_dbcluster_ii");
    stic = new statistics();
    
    if(NULL == mysql_conf || NULL == stic)
    {
        cout << "failed";
        return 1;
    }    
    if(!stic->init_statistics())
    {
        cout << "failed";
        return 1;
    }   
    stic->print_all();
    delete mysql_conf;
    delete stic;
    return 0;
}


int main(int argc, char **argv)
{
    if(argc)
        cout << argv[0];
    return cbo_test();
}

