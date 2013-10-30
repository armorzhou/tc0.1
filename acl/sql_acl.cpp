/* 
 * File:   SQL_ACL.cpp
 * Author: tangchao
 * 
 * Created on 2013年8月14日, 下午1:24
 */

#include <sys/types.h>
#include "../connector/mysql_con.h"
#include "../log/jlog.h"
#include "privileges.h"
#include "acl_access.h"
#include "sql_acl.h"

SQL_ACL *SQL_ACL::acl = NULL;
char* SQL_ACL::get_user_privilege()
{
    return "SELECT `username`, `host`,  "
    "`password_sec`, `is_super` FROM `joy_user`";
}

char* SQL_ACL::get_db_privilege()
{
    return "SELECT  `username`, `host`, "
    "`table_schema`, `priv`,  `is_global` FROM `joy_db_pri`";
}

char *SQL_ACL::get_table_privilege()
{
    return "SELECT  `username`, `host`, "
    "`table_schema`, `table_name`, `priv` FROM `joy_table_pri`";
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
SQL_ACL::set_skip_grant(bool flag)
{
    this->skip_grant = flag;
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
SQL_ACL::get_skip_grant()
{
    return this->skip_grant;
}

/**************************************************************
  Function:     get_acl_size
  Description:  
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
long long
SQL_ACL::get_acl_size()
{
    return acl_user.size();
}

/**************************************************************
  Function:     get_user_key
  Description:  generate a user key.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
SCHEMA
SQL_ACL::get_user_key(string user, string host)
{
    SCHEMA schema(user, host);
    return schema;
}

SCHEMA
SQL_ACL::get_user_key(char* user, char* host)
{
    string cxx_user, cxx_host;
    cxx_user.assign(user);
    cxx_host.assign(host);
    SCHEMA schema(cxx_user, cxx_host);
    return schema;
}

/**************************************************************
  Function:     acl_init_user
  Description:  init user information, username, password etc
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if init success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::acl_init_user()
{
    MYSQL_RES *res;
    MYSQL_ROW row;

    jlog(INFO, "enter SQL_ACL::acl_init_user");

    res = (MYSQL_RES *) meta_conn->ng_query(this->get_user_privilege());

    if (NULL == res)
    {
        return false;
    }

    row = mysql_fetch_row(res);
    while (row)
    {
        if (NULL == row[0] || NULL == row[1] || NULL == row[2] || NULL == row[3])
        {
            jlog(ERROR, "invalid user.");
            continue;
        }
        /* put user into ACL */
        if (!create_one_schema(get_user_key(row[0], row[1]), row[2], atoi(row[3]) ? true : false))
        {
            jlog(WARNING, "create user error.");
        }
        row = mysql_fetch_row(res);
    }

    mysql_free_result(res);
    return true;
}

/**************************************************************
  Function:     acl_init_db
  Description:  init database privileges of users
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if init success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::acl_init_db()
{
    MYSQL_RES *res;
    MYSQL_ROW row;

    jlog(INFO, "enter SQL_ACL::acl_init_user");

    res = (MYSQL_RES *) meta_conn->ng_query(this->get_db_privilege());

    if (NULL == res)
    {
        return false;
    }

    row = mysql_fetch_row(res);
    while (row)
    {
        /* row 0 username, 1 host, 2 db, 3 privilege, 4 level */
        if (NULL == row[0] || NULL == row[1] || NULL == row[2]
                || NULL == row[3] || NULL == row[4])
        {
            jlog(ERROR, "invalid user.");
            continue;
        }

        if (!put_db_privilege_into_schema(get_user_key(row[0], row[1]),
                row[2], atoi(row[4]) ? true : false,
                get_convert_pri(row[3])))
        {
            jlog(INFO, "init ACL error.");
        }
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    return true;
}

/**************************************************************
  Function:     acl_init_table
  Description:  init table privileges of users
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if init success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::acl_init_table()
{
    MYSQL_RES *res;
    MYSQL_ROW row;

    jlog(INFO, "enter SQL_ACL::acl_init_user");

    res = (MYSQL_RES *) meta_conn->ng_query(this->get_table_privilege());

    if (NULL == res)
    {
        return false;
    }

    row = mysql_fetch_row(res);
    while (row)
    {
        /* row 0 username, 1 host, 2 table_schema, 3 table_name, 4 priv */
        if (!put_table_privilege_into_schema(row[0], row[1],
                row[2], row[3], row[4]))
        {
            jlog(ERROR, "init ACL error, user %s@%s, table %s.%s, privilege %s",
                    row[0], row[1], row[2], row[3], row[4]);
            break;
        }
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    return true;
}

/**************************************************************
  Function:     SQL_ACL_init_auxiliary
  Description:  wrapper of user and privileges init
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if init success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::init()
{
    if (this->skip_grant)
    {
        jlog(WARNING, "skip ACL.");
        return true;
    }
    if (acl_init_user() && acl_init_db() && acl_init_table())
    {
        return true;
    }
    return false;
}

/**************************************************************
  Function:     reset
  Description:  ACL information reset. if skip_grant is true, we do not reset,
       otherwise do reset.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::reset()
{
    if (true == skip_grant)
    {
        jlog(WARNING, "skip ACL.");
        return true;
    }

    pthread_mutex_lock(&sql_acl_lock);
    sql_acl_clean();
    if (!init())
    {
        jlog(ERROR, "Reset ACL error");
    }
    pthread_mutex_unlock(&sql_acl_lock);
    return true;
}

/**************************************************************
  Function:     SQL_ACL_reset
  Description:  ACL information reset.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if success, otherwise return false 
  Other:
 **************************************************************/
void
SQL_ACL::sql_acl_clean()
{
    this->acl_user.clear();
    return;
}

/**************************************************************
  Function:     create_one_schema
  Description:  create a user if user not exist.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if create success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::create_one_schema(SCHEMA schema, string password, bool flag)
{
    ACL_SCHEMA *acl_schema;

    if (password.size() == 0)
    {
        jlog(ERROR, "invalid password.");
        return false;
    }

    if (schema.get_username().size() == 0 || schema.get_host().size() == 0)
    {
        jlog(ERROR, "invalid schema.");
        return false;
    }
    jlog(INFO, "create user %s@%s.", schema.get_username().c_str(),
            schema.get_host().c_str());

    acl_schema = this->find_user(schema);
    if (acl_schema)
    {
        jlog(WARNING, "user %s@%s exist.",
                schema.get_username().c_str(), schema.get_host().c_str());
        return false;
    }

    if (!this->add_one_user(schema, password, flag, NONE_PRI))
    {
        jlog(ERROR, "Create user %s@%s failed.", schema.get_username().c_str(),
                schema.get_host().c_str());
        return false;
    }

    return true;
}

/**************************************************************
  Function:     add_db_to_user
  Description:  add database privileges into user ACL resouce.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if add success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::put_db_privilege_into_schema(SCHEMA schema, string db_name,
        bool flag, long long priv)
{
    ACL_SCHEMA *acl_schema;
    ACL_DB *db;

    if (db_name.size() == 0)
    {
        jlog(ERROR, "invalid database name.");
        return false;
    }

    if (schema.get_username().size() == 0 || schema.get_host().size() == 0)
    {
        jlog(ERROR, "invalid user.");
        return false;
    }

    if (!check_privilege(priv))
    {
        return false;
    }

    acl_schema = find_user(schema);
    if (NULL == acl_schema)
    {
        jlog(ERROR, "%s@%s not exist.", schema.get_username().c_str(),
                schema.get_host().c_str());
        return false;
    }

    db = acl_schema->find_db(db_name);
    if (NULL == db)
    {
        return acl_schema->add_one_db(db_name, flag, priv);
    }
    return db->add_privilege(priv);
}

/**************************************************************
  Function:     add_table_to_user
  Description:  add table privileges into user ACL resouce.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if add success, otherwise return false 
  Other:
 **************************************************************/
bool
SQL_ACL::put_table_privilege_into_schema(char *user, char*host, char *db_name,
        char *table_name, char *privilege)
{
    ACL_SCHEMA *acl_schema;
    ACL_DB *db;

    if (NULL == user || NULL == host || NULL == db_name
            || NULL == table_name || NULL == privilege)
    {
        jlog(ERROR, "invalid pointer.");
    }

    SCHEMA schema = this->get_user_key(user, host);
    acl_schema = this->find_user(schema);
    if (NULL == acl_schema)
    {
        jlog(WARNING, "privilege table error, please check your privilege table.");
        return false;
    }

    db = acl_schema->find_db(db_name);
    if (NULL == db)
    {
        jlog(ERROR, "privilege table error, database %s error.", db_name);
        return false;
    }

    if (!db->find_table(table_name))
    {
        return db->create_table_pri(table_name, get_convert_pri(privilege));
    }
    return db->add_table_pri(table_name, get_convert_pri(privilege));
}

/**************************************************************
  Function:     find_user
  Description:  find user
  Author: 
  Date:         
  Input: 
  Output: 
  Return: NONULL user exist, otherwise return NULL  .
  Other:
 **************************************************************/
ACL_SCHEMA*
SQL_ACL::find_user(SCHEMA schema)
{
    map<SCHEMA, ACL_SCHEMA>::iterator iter;
    iter = acl_user.find(schema);
    return (iter == acl_user.end()) ? NULL : &iter->second;
}

ACL_SCHEMA*
SQL_ACL::find_user(SCHEMA *schema)
{
    return this->find_user(*schema);
}

/**************************************************************
  Function:     add_user_auxiliary
  Description:  add user into ACL information
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true if add user success, otherwise return false  .
  Other:
 **************************************************************/
bool
SQL_ACL::add_one_user(SCHEMA schema, string password, bool flag, long long priv)
{
    pair < map<SCHEMA, ACL_SCHEMA>::iterator, bool> insert_pair;

    if (password.size() == 0)
    {
        jlog(ERROR, "invalid password.");
        return false;
    }

    if (schema.get_username().size() == 0 || schema.get_host().size() == 0)

        if (!check_privilege(priv))
        {
            return false;
        }

    insert_pair = acl_user.insert(pair<SCHEMA, ACL_SCHEMA>(schema,
            ACL_SCHEMA(flag, schema.get_username(), schema.get_host(),
            password, priv)));
    return insert_pair.second;
}

/**************************************************************
  Function:     login_check_host
  Description:  acl check host before login
  Author: 
  Date:     
  Input: 
  Output: 
  Return: if allowed return true, otherwise return false.
  Other:
 **************************************************************/
bool
SQL_ACL::acl_check_host(char *ip)
{
    ACL_SCHEMA *acl_schema;
    map<SCHEMA, ACL_SCHEMA>::iterator iter;

    if (NULL == ip)
    {
        jlog(ERROR, "invalid pointer.");
        return false;
    }

    for (iter = acl_user.begin(); iter != acl_user.end(); iter++)
    {
        acl_schema = &iter->second;
        if (acl_schema->get_host().size() && 0 == acl_schema->get_host().compare(ip))
        {
            return true;
        }
    }
    return false;
}

/**************************************************************
  Function:print
  Description:  
  Author: 
  Date:     
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
void
SQL_ACL::print()
{
    ACL_SCHEMA *schema;

    map<SCHEMA, ACL_SCHEMA>::iterator iter;

    jlog(INFO, "user list: %lu users.", acl_user.size());
    for (iter = acl_user.begin(); iter != acl_user.end(); iter++)
    {
        schema = &iter->second;
        schema->print();
    }
    jlog(INFO, "list sucess.");

    return;
}
