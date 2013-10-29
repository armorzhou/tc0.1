/* 
 * File:   test_schema.cpp
 * Author: tangchao
 *
 * Created on 2013年10月24日, 上午11:20
 */

#include <cstdlib>
#include "../privileges.h"
#include "../acl_schema.h"

using namespace std;

/*
 * 
 */
int
main(int argc, char** argv)
{
    ACL_DB *db;
    ACL_SCHEMA *schema = new ACL_SCHEMA(false, "root", "192.168.1.1",
            "*449DEF63137E0FD9CA8307BC3A512B2D55E5A657", 1);
    schema->add_one_db("tuan", true, SELECT_PRI);
    schema->add_one_db("qinglong", true, SELECT_PRI);
    schema->add_one_db("userinfo", true, SELECT_PRI);
    db = schema->find_db("tuan");
    if (NULL == db)
    {
        cout << "find tuan error\n";
    }
    db->add_one_table("table1", 2);
    db->add_one_table("table2", 4);
    db->add_one_table("table3", 8);
    db->add_table_pri("table3", 2);
    db->add_table_pri("table3", 4);
    if (!db->find_table("table1"))
    {
        cout << "find table1 error" << endl;
    }
    if (!db->find_table("table2"))
    {
        cout << "find table2 error" << endl;
    }
    if (!db->find_table("table3"))
    {
        cout << "find table3 error" << endl;
    }
    schema->print();
    delete schema;

    return 0;
}

