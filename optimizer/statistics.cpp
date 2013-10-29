/* 
 * File:   statistics.cpp
 * Author: tangchao
 * 
 * Created on 2013年8月9日, 上午11:36
 */

#include "../log/jlog.h"
#include "../connector/mysql_con.h"
#include "statistics.h"

static char *information_schema = "INFORMATION_SCHEMA";
static char *table_status = "SELECT jt.TABLE_SCHEMA as table_schema, jt.TABLE_NAME as table_name,"
        "jt.TABLE_ROWS as table_rows, st.column_name as column_name, "
        "st.`cardinality` as cardinality"
        " FROM joy_tables jt JOIN statistics st "
        "ON jt.`TABLE_SCHEMA` = st.`table_schema` AND  jt.`TABLE_NAME` = st.`table_name`";

/**************************************************************
  Function:     close_statistics
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
statistics::close_statistics()
{
    this->health = false;
}

/**************************************************************
  Function:     open_statistics
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
statistics::open_statistics()
{
    this->health = true;
}

/**************************************************************
  Function:     init_information_schema
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::init_information_schema()
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    int rows, fields;

    jlog(INFO, "enter SQL_ACL::acl_init_user");

    res = (MYSQL_RES *) meta_conn->ng_query(table_status);

    if (NULL == res)
    {
        return false;
    }

    fields = mysql_num_fields(res);
    rows = mysql_num_rows(res);
    field = mysql_fetch_fields(res);
    if (field == NULL)
    {
        mysql_free_result(res);
        return false;
    }

    row = mysql_fetch_row(res);
    while (row)
    {
        /* row 0 db, 1 table, 2 rows, 3 field, 4 cardinality */
        if (NULL == row[0] || NULL == row[1] || NULL == row[2]
                || NULL == row[3] || NULL == row[4])
        {
            jlog(ERROR, "invalid data.");
            continue;
        }

        jlog(INFO, "statistics init table[%s.%s], rows:%s, column:%s, cardinality:%s.",
                row[0], row[1], row[2], row[3], row[4]);

        if (update_cardinality(row[0], row[1], row[2], atoi(row[3]), atoi(row[4])))
        {

            jlog(INFO, "init statistics success.");
        }
        else
        {

            jlog(ERROR, "init statistics failed.");

        }

        row = mysql_fetch_row(res);
    }

    mysql_free_result(res);
    return true;
}

/**************************************************************
  Function:     init_statistics
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::init_statistics()
{
    if (init_information_schema())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/***************************************************************************/
/* 统计信息更新 */
/***************************************************************************/

/**************************************************************
  Function:     update_table
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::update_table(class cs_table *db, class cs_field *table,
        string field_name, long long cardinality)
{
    jlog(INFO, "enter statistics->update_table");

    if (NULL == db || NULL == table)
    {
        jlog(ERROR, "invalid calling");
        return false;
    }

    if (this->find_field(field_name, table))
    {
        jlog(INFO, "update cardinality:table->%s, %lld",
                table->get_table_name().c_str(), cardinality);

        table->field_map[field_name] = cardinality;
        return true;
    }
    else
    {
        jlog(INFO, "%s.%s not exist",
                table->get_table_name().c_str(), field_name.c_str());
        return this->add_field(field_name, cardinality, table);
    }
}

/**************************************************************
  Function:     update_table
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::update_table(class cs_table *db, string table_name,
        string field_name,
        long long table_rows,
        long long cardinality)
{
    class cs_field *table = NULL;

    jlog(INFO, "enter statistics::update_table");

    if (!db)
    {
        jlog(ERROR, "Hit a bug.");
        return false;
    }

    if (this->add_table(table_name, db, table_rows))
    {
        table = this->find_table(table_name, db);

        if (this->find_field(field_name, table))
        {
            table->field_map[field_name] = cardinality;
            return true;
        }
        else
        {
            return this->add_field(field_name, cardinality, table);
        }
    }
    else
    {
        return false;
    }
}

/**************************************************************
  Function:     update_db
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::update_db(class cs_table *db, string table_name,
        string field_name,
        long long table_rows,
        long long cardinality)
{
    class cs_field *table = NULL;

    jlog(INFO, "enter statistics::update_db.");
    jlog(INFO, "table:%s.%s.", (NULL == db) ? NULL : db->get_db_name().c_str(),
            table_name.c_str());

    if (NULL == db)
    {
        jlog(ERROR, "invalid var , it's a bug ");
        return false;
    }

    table = this->find_table(table_name, db);

    jlog(INFO, "table:%s", (NULL == table) ? NULL : table->get_table_name().c_str());

    if (NULL != table)
    {
        jlog(INFO, "table:%s exist", table->get_table_name().c_str());

        table->set_table_rows(table_rows);
        return update_table(db, table, field_name, cardinality);
    }
    else
    {
        jlog(INFO, "table:%s not exist, add it new", table_name.c_str());
        return update_table(db, table_name, field_name, table_rows, cardinality);
    }
}

/**************************************************************
  Function:     update_db
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::update_db(string db_name, string table_name, string field_name,
        long long table_rows, long long cardinality)
{
    class cs_table *db = NULL;

    jlog(INFO, "enter statistics::update_db");

    if (this->add_db(db_name))
    {
        db = this->find_db(db_name);

        if (db)
        {
            jlog(INFO, "add new db:%s success.",
                    (NULL == db) ? NULL : db->get_db_name().c_str());
            return update_db(db, table_name, field_name, table_rows, cardinality);
        }
        else
        {
            jlog(INFO, "Hit a bug");
            return false;
        }
    }
    else
    {
        jlog(INFO, "add new db:%s failed.", db_name.c_str());
        return false;
    }
}

/**************************************************************
  Function:     update_cardinality
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::update_cardinality(string db_name, string table_name,
        string field_name, long long table_rows,
        long long cardinality)
{
    class cs_table *db = NULL;
    jlog(INFO, "enter statistics::update_cardinality()");
    jlog(INFO, "table:%s.%s, field:%s, rows:%lld, cardinality:%lld.",
            db_name.c_str(), table_name.c_str(), field_name.c_str(),
            table_rows, cardinality);

    db = this->find_db(db_name);
    if (db)
    {
        jlog(INFO, "db:%s", db->get_db_name().c_str());

        return update_db(db, table_name, field_name, table_rows, cardinality);
    }
    else
    {
        jlog(INFO, "db:%s not exist, add it now.", db_name.c_str());

        return update_db(db_name, table_name, field_name, table_rows, cardinality);
    }
}

/**************************************************************
  Function:     add_db
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::add_db(string db_name)
{
    pair < map<string_db_name, class cs_table>::iterator, bool> insert_pair;

    if (this->find_db(db_name))
    {
        jlog(ERROR, "[statistics] add database->%s failed, database exist",
                db_name.c_str());
    }

    insert_pair = this->db_map.insert(
            map<string_db_name, class cs_table >::value_type(db_name, cs_table(db_name)));

    return insert_pair.second;
}

/**************************************************************
  Function:     del_db
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::del_db(string db_name)
{
    map<string_db_name, class cs_table>::iterator iter;

    iter = this->db_map.find(db_name);
    if (iter == this->db_map.end())
    {
        jlog(ERROR, "database:%s not exist", db_name.c_str());
        return false;
    }
    else
    {
        this->db_map.erase(iter);
        return true;
    }
}

/**************************************************************
  Function:     find_db
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
cs_table*
statistics::find_db(string db_name)
{
    map<string_db_name, class cs_table>::iterator iter;

    iter = this->db_map.find(db_name);
    if (iter == this->db_map.end())
    {
        return NULL;
    }
    else
    {
        return &iter->second;
    }
}

/**************************************************************
  Function:     add_table
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::add_table(string table_name, cs_table *table,
        long long table_rows)
{
    pair < map<string_table_name, class cs_field>::iterator, bool> insert_pair;

    jlog(INFO, "enter statistics::add_table");

    if (NULL == table)
    {
        jlog(ERROR, "Hit a bug.");
        return false;
    }

    if (this->find_table(table_name, table))
    {
        jlog(ERROR, "Hit a bug, table:%s exist.", table_name.c_str());
        return false;
    }

    insert_pair = table->table_map.insert(
            pair<string_table_name, class cs_field > (table_name, cs_field(table_name, table_rows)));
    jlog(INFO, "Add table:%s success.", table_name.c_str());
    return insert_pair.second;
}

/**************************************************************
  Function:     del_table
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::del_table(string table_name, class cs_table *table)
{
    map<string_table_name, class cs_field>::iterator iter;

    if (NULL == table)
    {
        jlog(ERROR, "Hit a bug");
        return false;
    }

    iter = table->table_map.find(table_name);
    if (iter == table->table_map.end())
    {
        jlog(ERROR, "Delete table:%s failed, not exist.", table_name.c_str());
        return false;
    }
    else
    {
        table->table_map.erase(iter);
        return true;
    }
}

/**************************************************************
  Function:     find_table
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
cs_field*
statistics::find_table(string table_name, cs_table *table)
{
    map<string_table_name, class cs_field>::iterator iter;

    if (NULL == table)
    {
        jlog(ERROR, "Hit a bug");
        return NULL;
    }

    iter = table->table_map.find(table_name);
    if (iter == table->table_map.end())
    {
        jlog(INFO, "table:%s not exist.", table_name.c_str());
        return NULL;
    }
    else
    {
        jlog(INFO, "table:%s exist.", table_name.c_str());
        return &iter->second;
    }
}

/**************************************************************
  Function:     add_field
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::add_field(string field_name, long long cardinality, cs_field *field)
{
    pair < map<string_column_name, long long>::iterator, bool> insert_pair;

    if (NULL == field)
    {
        jlog(ERROR, "Hit a bug");
        return false;
    }

    insert_pair = field->field_map.insert(
            pair<string_column_name, long long>(field_name, cardinality));

    return insert_pair.second;
}

/**************************************************************
  Function:     del_field
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::del_field(string field_name, class cs_field *field)
{
    map<string_column_name, long long>::iterator iter;

    if (NULL == field)
    {
        jlog(ERROR, "Hit a bug");
        return false;
    }

    iter = field->field_map.find(field_name);
    if (iter == field->field_map.end())
    {
        jlog(ERROR, "delete field:%s from %s", field_name.c_str(),
                field->get_table_name().c_str());
        return false;
    }
    else
    {
        field->field_map.erase(iter);
        return true;
    }
}

/**************************************************************
  Function:     find_field
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::find_field(string field_name, class cs_field *field)
{
    map<string_column_name, long long>::iterator iter;

    jlog(INFO, "enter find_field");

    if (!field)
    {
        jlog(ERROR, "Hit a bug");
        return false;
    }

    iter = field->field_map.find(field_name);
    if (iter == field->field_map.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**************************************************************
  Function:     get_cardinality
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
long long
statistics::get_cardinality(string field_name, class cs_field *field)
{
    map<string_column_name, long long>::iterator iter;

    if (NULL == field)
    {
        jlog(ERROR, "Hit a bug");
        return -1;
    }

    iter = field->field_map.find(field_name);
    if (iter == field->field_map.end())
    {
        return -1;
    }
    else
    {
        return iter->second;
    }
}

/**************************************************************
  Function:     statistics_size
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
long long
statistics::statistics_size()
{
    return db_map.size();
}

/**************************************************************
  Function:     reset_statistics
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::reset_statistics(class mysql_meta *meta_info)
{
    map<string_db_name, class cs_table>::iterator iter;

    for (iter = this->db_map.begin(); iter != this->db_map.end(); iter++)
    {
        this->db_map.erase(iter);
    }
    return this->init_statistics(meta_info);
}

/**************************************************************
  Function:     reset
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::reset(class mysql_meta *meta_info)
{
    this->close_statistics();
    if (this->reset_statistics(meta_info))
    {
        this->open_statistics();
        return true;
    }
    else
    {
        jlog(ERROR, "Hit a bug");
        return false;
    }
}

/**************************************************************
  Function:     get_statistics_health
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
statistics::get_statistics_health()
{
    return this->health;
}

/**************************************************************
  Function:     get_reckon_rows
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: -1 error, > -1 success
  Other:
 **************************************************************/
long long
statistics::get_reckon_rows(string db_name, string table_name, string field_name)
{
    class cs_table *db = NULL;
    class cs_field *table = NULL;

    jlog(INFO, "enter statistics::get_reckon_rows");

    db = this->find_db(db_name);
    if (NULL == db)
    {
        jlog(WARNING, "db:%s not exist.", db_name.c_str());
        return -1;
    }

    table = this->find_table(table_name, db);
    if (NULL == table)
    {
        jlog(ERROR, "table:%s not exist.", table_name.c_str());
        return -1;
    }

    if (this->find_field(field_name, table))
    {
        return this->get_cardinality(field_name, table);

    }
    else
    {
        jlog(ERROR, "Field:%s not exist.", field_name.c_str());
        return -1;
    }
}

/**************************************************************
  Function:     print_all
  Description:   print all table statistics
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
statistics::print_all()
{
    class cs_table *db = NULL;
    class cs_field *table = NULL;

    map<string_db_name, class cs_table>::iterator iter_db;
    map<string_table_name, class cs_field>::iterator iter_table;
    map<string_column_name, long long>::iterator iter_field;

    jlog(INFO, "<=<=<=<=<=<=<statistics information>=>=>=>=>=>=>=>");
    for (iter_db = this->db_map.begin();
            iter_db != this->db_map.end(); iter_db++)
    {
        db = &iter_db->second;
        jlog(INFO, "db:%s", db->get_db_name().c_str());
        for (iter_table = db->table_map.begin();
                iter_table != db->table_map.end(); iter_table++)
        {
            table = &iter_table->second;
            jlog(INFO, "\ttable:%s, table rows:%lld",
                    table->get_table_name().c_str(),
                    table->get_table_rows());
            for (iter_field = table->field_map.begin();
                    iter_field != table->field_map.end(); iter_field++)
            {
                jlog(INFO, "\t\tfield:%s, cardinality:%lld",
                        iter_field->first.c_str(), iter_field->second);
            }
        }
    }

    jlog(INFO, "print statistics success");
    return;
}
