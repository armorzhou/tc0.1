/* 
 * File:   main.cpp
 * Author: tangchao
 *
 * Created on 2013年8月15日, 下午7:05
 */

#include <cstdlib>
#include "../acl_db.h"

using namespace std;

int
main(void)
{
    ACL_DB *db = new ACL_DB("jingdong", false, 14);
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
    db->print();
    delete db;
    return 0;
}
