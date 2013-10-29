/*
 * File:   sql_schema.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月6日, 下午7:23
 */

#include <vector>
#include "../log/jlog.h"
#include "acl_convert.h"
#include "acl_schema.h"

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
void
ACL_SCHEMA::set_username(string username)
{
    this->username = username;
    return;
}

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
string
ACL_SCHEMA::get_username()
{
    return this->username;
}

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
void
ACL_SCHEMA::set_super(bool flag)
{
    this->super = flag;
    return;
}

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
bool
ACL_SCHEMA::get_super()
{
    return this->super;
}

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
string
ACL_SCHEMA::get_password()
{
    return this->password;
}

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
void
ACL_SCHEMA::set_host(string host)
{
    this->host = host;
    return;
}

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
string
ACL_SCHEMA::get_host()
{
    return this->host;
}

/**************************************************************
  Function:     
  Description:   Convert scrambled password from asciiz hex string to binary form.

  SYNOPSIS
    get_salt_from_password()
    res       OUT buf to hold password. Must be at least SHA1_HASH_SIZE
                  bytes long.
    password  IN  4.1.1 version value of user.password
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
void
ACL_SCHEMA::tc_get_salt_from_password(unsigned char *hash_stage2,
        const char *password)
{
    get_salt_from_password(hash_stage2, password);
}

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
unsigned char*
ACL_SCHEMA::get_salt()
{
    return this->salt;
}

/**************************************************************
  Function:     find_db
  Description:  find db from user resouce
  Author: 
  Date:         
  Input: 
  Output: 
  Return: NONULL user exist, otherwise return NULL  .
  Other:
 **************************************************************/
ACL_DB*
ACL_SCHEMA::find_db(string db_name)
{
    map<string, ACL_DB>::iterator db;

    if (db_name.size() == 0)
    {
        jlog(WARNING, "invalid database name.");
        return NULL;
    }
    db = this->schema.find(db_name);
    if (db == this->schema.end())
    {
        return NULL;
    }
    else
    {
        return &db->second;
    }
}

ACL_DB*
ACL_SCHEMA::find_db(char *db_name)
{
    string db;
    if (NULL == db_name)
    {
        db.assign("");
    }
    else
    {
        db.assign(db_name);
    }
    return this->find_db(db);
}

/**************************************************************
  Function:     check_db
  Description:  check user privilege for database
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true user can access db, else return false  .
  Other:
 **************************************************************/
bool
ACL_SCHEMA::check_db(string db_name, long long priv)
{
    ACL_DB *db;

    if (this->get_super())
    {
        return true;
    }

    if (!check_privilege(priv))
    {
        return false;
    }

    db = this->find_db(db_name);
    if (db && (db->get_privilege() & priv))
    {
        return true;
    }
    return false;
}

/**************************************************************
  Function:     check_table
  Description:  check user privilege for table. If user is a super user,  
 need only hold a valid access privilege. 
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true user can access table, else return false.
  Other:
 **************************************************************/
bool
ACL_SCHEMA::check_table(string db_name, string table_name, long long priv)
{
    ACL_DB *db;

    if (get_super())
    {
        return true;
    }

    if (!check_privilege(priv))
    {
        return false;
    }

    db = this->find_db(db_name);
    if (NULL == db)
    {

        jlog(WARNING, "Access denied, database %s, privilege %s, user %s@%s",
                db_name.c_str(), get_string_pri(priv),
                username.c_str(), password.c_str());
        return false;
    }

    if (db->get_access_all_tables() && (db->get_privilege() & priv))
    {
        return true;
    }

    if(db->find_table(table_name) && db->get_table_pri(table_name) & priv)
    {
        return true;
    }

    jlog(WARNING, "Access denied, object %s.%s, privilege %s, user %s@%s",
            db_name.c_str(), table_name.c_str(), get_string_pri(priv),
            username.c_str(), host.c_str());
    return false;
}

/**************************************************************
  Function:     add_one_db
  Description:
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true user can access table, else return false.
  Other:
 **************************************************************/
bool
ACL_SCHEMA::add_one_db(string db_name, bool level, long long privilege)
{
    pair < map<string, ACL_DB>::iterator, bool > add_pair;

    if (db_name.size() == 0)
    {
        jlog(ERROR, "database name must have at least one character at a time.");
        return false;
    }

    if (!check_privilege(privilege))
    {
        return false;
    }

    add_pair = this->schema.insert(pair<string, ACL_DB>
            (db_name, ACL_DB(db_name, level, privilege)));
    return add_pair.second;
}

/**************************************************************
  Function:     delete_one_db
  Description: 
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true user can access table, else return false.
  Other:
 **************************************************************/
void
ACL_SCHEMA::delete_one_db(string *db_name)
{
    schema.erase(*db_name);
}

/**************************************************************
  Function:     get_schema
  Description: 
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true user can access table, else return false.
  Other:
 **************************************************************/
void
ACL_SCHEMA::print()
{
    map<string, ACL_DB>::iterator iter;
    ACL_DB *db;
    
    if(this->username.size() == 0)
    {
        jlog(WARNING, "username size is 0\n");
        return;
    }
    
    jlog(INFO, "%s@%s\t%s\t%s",
            this->username.size() ? this->username.c_str() : NULL,
            this->host.size() ? this->host.c_str() : NULL,
            this->password.size() ? this->password.c_str() : NULL,
            this->super ? "super" : "normal");
    
    for(iter = this->schema.begin(); iter != this->schema.end(); iter++)
    {
        db = &iter->second;
        db->print();
    }  
    return;
}
