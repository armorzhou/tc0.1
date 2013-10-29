/* 
 * File:   stmt_template.cpp
 * Author: tangchao
 * 
 * Created on 2013年8月16日, 上午10:51
 */

#include "stmt_template.h"
#include "../log/jlog.h"

/***************************************************************************/
/* class relation_algorithm */

/***************************************************************************/
void
relation_algorithm::set_algor(enum relation_algor algor)
{
    this->algor = algor;
    return;
}

enum relation_algor
relation_algorithm::get_algor()
{
    return this->algor;
}

/***************************************************************************/
/* class relation_table */

/***************************************************************************/
void
relation_table::set_table_name(string table_name)
{
    this->table_name.assign(table_name);
    return;
}

string
relation_table::get_table_name()
{
    return this->table_name;
}

void
relation_table::set_column_name(string column_name)
{
    this->column_name.assign(column_name);
    return;
}

string
relation_table::get_column_name()
{
    return this->column_name;
}

/***************************************************************************/
/* class column_template */

/***************************************************************************/
void
column_template::set_column_value(string column_value)
{
    this->column_value.assign(column_value);
    return;
}

string
column_template::get_column_value()
{
    return this->column_value;
}

/***************************************************************************/
/* class stmt_template */

/***************************************************************************/
void
stmt_template::set_join(enum JOIN_TYPE join)
{
    this->join = join;
    return;
}

enum JOIN_TYPE
stmt_template::get_join()
{
    return this->join;
}
