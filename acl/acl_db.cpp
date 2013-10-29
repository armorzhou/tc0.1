/* 
 * File:   ACL_DB.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月6日, 下午7:27
 */

#include "../log/jlog.h"
#include "acl_convert.h"
#include "privileges.h"
#include "acl_db.h"

/**************************************************************
  Function:find_table
  Description: 
  Author: 
  Date:         
  Input: 
  Output: 
  Return:no found return NULL, 
         otherwise return a pointer point to ACL_TABLE.
  Other:
 **************************************************************/
bool
ACL_DB::find_table(string table_name)
{
    map<string, long long>::iterator iter;

    if (table_name.size() == 0)
    {
        jlog(ERROR, "table name [], table name must be a charactor");
    }

    iter = tables.find(table_name);
    if (iter == tables.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool
ACL_DB::find_table(char* table_name)
{
    string table;
    if (NULL == table_name)
    {
        table.assign("");
    }
    else
    {
        table.assign(table_name);
    }
    return this->find_table(table);
}

/**************************************************************
  Function:add_one_table
  Description:
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
bool
ACL_DB::add_one_table(string table_name, long long priv)
{
    pair < map<string, long long>::iterator, bool > add_pair;

    if (table_name.size() == 0)
    {
        jlog(ERROR, "table name must have at least one character at a time.");
        return false;
    }

    if (!check_privilege(priv))
    {
        return false;
    }

    add_pair = this->tables.insert(pair<string, long long>
            (table_name, priv));
    return add_pair.second;
}

/**************************************************************
  Function:get_table_pri
  Description:
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
long long
ACL_DB::get_table_pri(string table_name)
{
    map<string, long long>::iterator iter;

    if (table_name.size() == 0)
    {
        jlog(ERROR, "table name [], table name must be a charactor");
    }

    iter = tables.find(table_name);
    if (iter == tables.end())
    {
        return NONE_PRI;
    }
    else
    {
        return iter->second;
    }
}

/**************************************************************
  Function:create_table_pri
  Description:
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
bool
ACL_DB::create_table_pri(string table_name, long long priv)
{
    pair <map<string, long long>::iterator, bool> insert_pair;

    if (table_name.size() == 0)
    {
        jlog(ERROR, "table name [], table name must be a charactor");
        return false;
    }

    if(!check_privilege(priv))
    {
        return false;
    }
    insert_pair = this->tables.insert(pair<string, long long>(table_name, priv));
    return insert_pair.second;
}

bool
ACL_DB::create_table_pri(char* table_name, long long priv)
{
    string table;
    if (NULL == table_name)
    {
        table.assign("");
    }
    else
    {
        table.assign(table_name);
    }
    return this->create_table_pri(table, priv);
}

/**************************************************************
  Function:add_table_pri
  Description:
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
bool
ACL_DB::add_table_pri(string table_name, long long priv)
{
    map<string, long long>::iterator iter;

    if (table_name.size() == 0)
    {
        jlog(ERROR, "table name [], table name must be a charactor");
    }

    if (!check_privilege(priv))
    {
        return false;
    }
    iter = tables.find(table_name);
    if (iter == tables.end())
    {
        return false;
    }
    else
    {
        iter->second += priv;
        return true;
    }
}

bool
ACL_DB::add_table_pri(char* table_name, long long priv)
{
    string table;
    if (NULL == table_name)
    {
        table.assign("");
    }
    else
    {
        table.assign(table_name);
    }
    return this->add_table_pri(table, priv);
}

/**************************************************************
  Function:fetch_one_db_table
  Description:fetch all tables in the specified database
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
void
ACL_DB::print()
{
    map<string, long long>::iterator iter;

    if (this->db_name.size() == 0)
    {
        jlog(WARNING, "db_name size is 0\n");
        return;
    }

    jlog(INFO, "\tdb:%s\tprivilege level:%s\tprivilege:%lld\ttables:%lu",
            this->db_name.c_str(),
            this->access_all_tables ? "db" : "table",
            this->get_privilege(),
            this->tables.size());

    for (iter = this->tables.begin(); iter != this->tables.end(); iter++)
    {
        jlog(INFO, "\t\ttable:%s\tprivilege:%lld",
                iter->first.c_str(), iter->second);
    }
    return;
}
