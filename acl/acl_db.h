/* 
 * File:   ACL_DB.h
 * Author: tangchao
 *
 * Created on 2013年9月6日, 下午7:27
 */

#ifndef ACL_DB_H
#define	ACL_DB_H
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <string>
#include <sys/types.h>
#include "acl_access.h"

using namespace std;

class ACL_DB : public ACL_ACCESS
{
public:

    ACL_DB(string db_name, bool flag, long long priv) :
    ACL_ACCESS(priv), db_name(db_name), access_all_tables(flag)
    {
    }

    ACL_DB(const ACL_DB& orig) :
    ACL_ACCESS(orig), db_name(orig.db_name), access_all_tables(orig.access_all_tables)
    {
    }

    virtual ~ACL_DB()
    {
    }

    void set_db_name(string db_name)
    {
        this->db_name.assign(db_name);
    }

    string get_db_name()
    {
        return db_name;
    }

    void set_access_all_tables(bool flag)
    {
        this->access_all_tables = flag;
    }

    bool get_access_all_tables()
    {
        return this->access_all_tables;
    }

    bool find_table(string table_name);
    bool find_table(char* table_name);
    bool create_table_pri(string table_name, long long priv);
    bool create_table_pri(char* table_name, long long priv);
    bool add_table_pri(string table_name, long long priv);
    bool add_table_pri(char* table_name, long long priv);
    bool add_one_table(string table_name, long long priv);

    long long get_table_pri(string table_name);
    ulong get_table_number()
    {
        return tables.size();
    }
    void print();
private:
    string db_name;
    bool access_all_tables; /* access level */
    map<string, long long> tables; /* tables */
};

#endif	/* ACL_DB_H */

