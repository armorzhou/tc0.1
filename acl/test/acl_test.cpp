/* 
 * File:   main.cpp
 * Author: tangchao
 *
 * Created on 2013年8月15日, 下午7:05
 */

#include <cstdlib>
#include "../../log/jlog.h"
#include "../sql_acl.h"
#include "../../connector/mysql_con.h"
#include "../privileges.h"

using namespace std;

int
main(void)
{
    ACL_SCHEMA *schema;
    meta_conn = new mysql_con();
    int i = 10;
    meta_conn->ng_create_connect("172.17.4.50", "dba", "yvhkfhvk", "joy_dbcluster_ii", 3333);
    
    SQL_ACL::get_instance(false)->init();
    
    while(i--)
    {
        SQL_ACL::get_instance(false)->reset();
        SQL_ACL::get_instance(false)->print();
    }
    
    schema = SQL_ACL::get_instance(false)->find_user(SCHEMA("lucy", "172.17.4.50"));
    
    if(!schema->check_db("erp", SELECT_PRI))
    {
        cout << "db erp, no select" << endl;
    }
    if(!schema->check_db("qinglong", SELECT_PRI))
    {
        cout << "db qinglong no select" << endl;
    }
    
    if(!schema->check_table("qinglong", "orderinfo", SELECT_PRI))
    {
        cout << "table qinglong.orderinfo no select\n";
    } 
    return 0;
}
