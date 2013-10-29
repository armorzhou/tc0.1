/* 
 * File:   object_select.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月30日, 下午2:13
 */

#include <vector>

#include "../log/jlog.h"
#include "../route/meta_reader.h"
#include "../tc_global.h"
#include "object_select.h"

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
object_select::query_store_data(mysql_select& pre_select)
{
    select.push_back(pre_select);
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
void
object_select::query_store_data(mysql_where& pre_where)
{
    where.push_back(pre_where);
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
void
object_select::query_store_data(mysql_group& pre_group)
{
    group.push_back(pre_group);
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
void
object_select::query_store_data(mysql_hav& pre_hav)
{
    hav.push_back(pre_hav);
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
void
object_select::query_store_data(mysql_order& pre_order)
{
    order.push_back(pre_order);
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
void
object_select::query_store_data(mysql_limit& pre_limit)
{
    limit.push_back(pre_limit);
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
void
object_select::query_store_data(mysql_table& pre_table)
{
    table.join_table.push_back(pre_table);
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
void
object_select::query_store_data(mysql_on& pre_on)
{
    on.push_back(pre_on);
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
void
object_select::query_store_select(string table_name, string field_name,
        SqlItemType aggregate_func)
{
    mysql_select mysql;
    mysql.table_name = table_name;
    mysql.field_name = field_name;
    mysql.aggregate_func = aggregate_func;
    query_store_data(mysql);
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
void
object_select::query_store_where(string table_name, string field_name)
{
    mysql_where mysql;
    mysql.table_name = table_name;
    mysql.field_name = field_name;
    query_store_data(mysql);
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
void
object_select::query_store_group(string table_name, string field_name,
        ushort pos, GroupItem::GroupType sort)
{
    mysql_group mysql;
    mysql.table_name = table_name;
    mysql.field_name = field_name;
    mysql.pos = pos;
    mysql.sort = sort;
    query_store_data(mysql);
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
void
object_select::query_store_hav(string str)
{
    mysql_hav mysql;
    mysql.str = str;
    query_store_data(mysql);
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
void
object_select::query_store_order(string table_name, string field_name,
        ushort pos, OrderItem::OrderType sort)
{
    mysql_order mysql;
    mysql.table_name = table_name;
    mysql.field_name = field_name;
    mysql.pos = pos;
    mysql.sort = sort;
    query_store_data(mysql);
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
void
object_select::query_store_limit(ulong start, ulong end, ushort end_of_pos)
{
    mysql_limit mysql;
    mysql.start = start;
    mysql.end = end;
    mysql.end_of_pos = end_of_pos;
    query_store_data(mysql);
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
void
object_select::query_store_join(string db_name, string table_name,
        ushort pos, string opera_str, bool j)
{
    mysql_table mysql;
    mysql.db_name = db_name;
    mysql.table_name = table_name;
    mysql.pos = pos;
    query_store_data(mysql);
    this->table.opera_str = opera_str;
    this->table.have_join = j;
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
void
object_select::query_store_on(string table_name, string field_name,
        ushort pos, string opera_str)
{
    mysql_on mysql;
    mysql.table_name = table_name;
    mysql.field_name = field_name;
    mysql.pos = pos;
    mysql.opera_str = opera_str;
    query_store_data(mysql);
}

/**************************************************************
  Function:check_sqlstree_alive 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return:return false sqltree is alive. return true sqltree is invalid.
  Other:
 **************************************************************/
bool
object_select::check_sqlstree_alive()
{
    vector<string> tables;
    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "query no table");
        return true;
    }

    if (tables.size() > ALLOW_TABLES_IN_ONE_SQL)
    {
        jlog(WARNING, "Does not support more than %d tables", ALLOW_TABLES_IN_ONE_SQL);
        return true;
    }
    return false;
}

/**************************************************************
  Function:fetch_physics_name_from_field 
  Description:fetch physics field name from [SelectItem].
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
string
object_select::fetch_physics_name_from_field(SelectItem *field)
{
    if (field)
        return field->raw_select_item_name;
    else
        return "";
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
object_select::fetch_physics_name_from_order(OrderItem *field)
{
    if (field)
        return field->order_column;
    else
        return "";
}

/**************************************************************
  Function:fetch_sort_from_order 
  Description:fetch sort from [OrderItem], convert sort from [ASC|DESC] to bool.
  Author:
  Date:
  Input:
  Output:
  Return:true ASC, false DESC;
  Other:
 **************************************************************/
bool
object_select::fetch_sort_from_order(OrderItem *field)
{
    if (field)
        return field->order_type_ ? 1 : 0;
    else
    {
        cout << "WARNING, OrderItem* is null";
        return -1;
    }
}

/**************************************************************
  Function:fetch_physics_name_from_group 
  Description:fetch physics field name from [GroupItem].  
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
string
object_select::fetch_physics_name_from_group(GroupItem *field)
{
    if (field)
        return field->group_column_;
    else
        return "";
}

/**************************************************************
  Function:fetch_aggregate_func_from_field 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
SqlItemType
object_select::fetch_aggregate_func_from_field(SelectItem *field)
{
    if (field)
        return field->aggr_fun_type;
    else
        return T_INVALID;
}

/**************************************************************
  Function:fix_bug_limit_no_order 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
object_select::fix_bug_limit_no_order()
{
    if (have_capabilities & SELECT_WITH_LIMIT &&
            !(have_capabilities & SELECT_WITH_ORDERBY))
    {
        obselect->set_order_by_primary(true);
    }
    else
    {
        obselect->set_order_by_primary(false);
    }
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
void
object_select::store_select_field(string table_name, SelectItem *field)
{
    if (!(this->have_capabilities & SELECT_WITH_AGG))
    {
        query_store_select(table_name, fetch_physics_name_from_field(field),
                T_INVALID /* value 0 */);
        return;
    }

    switch (fetch_aggregate_func_from_field(field))
    {
        case T_FUN_MAX:
        case T_FUN_MIN:
        case T_FUN_SUM:
        case T_FUN_COUNT:
        case T_FUN_AVG:
            query_store_select(table_name, fetch_physics_name_from_field(field),
                    fetch_aggregate_func_from_field(field));
            break;
        case T_INVALID:
            query_store_select(table_name, fetch_physics_name_from_field(field),
                    T_INVALID /* value 0 */);
            break;
        default:
            jlog(WARNING, "invalid aggregate function.");
            break;
    }
    return;
}

/**************************************************************
  Function: dump_select_item
  Description: dump SELECT item.... FROM. dump column name, aggregate function.
 select max(id), name from ...dump ok. 
 select if/when..& select id / idx we do not support.
  Author:
  Date:
  Input:
  Output:
  Return:dump success return true, otherwise return false. 
  Other:
 **************************************************************/
bool
object_select::dump_select_item()
{
    vector<string> tables;
    vector<SelectItem> select_str;
    ushort next, next_item;

    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "table size=%lu", tables.size());
        return false;
    }

    for (next = 0; next < tables.size(); next++)
    {
        select_str = obselect->fetch_select_from_tree(*result_plan, tables[next]);
        if (select_str.size() == 0)
        {
            jlog(WARNING, "select item size = %lu", select_str.size());
            return false;
        }
        for (next_item = 0; next_item < select_str.size(); next_item++)
        {
            store_select_field(tables[next], &select_str[next_item]);
        }
    }

    return true;
}

/**************************************************************
  Function:dump_table_item 
  Description:dump FROM ... WHERE, dump table name, join, on.
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_table_item()
{
    vector<string> tables;
    ushort next;

    if (check_sqlstree_alive())
    {
        return false;
    }

    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "table size=%lu", tables.size());
        return false;
    }
    for (next = 0; next < tables.size(); next++)
    {
        query_store_join(this->db, tables[next], 0, "", false);
    }
    return true;
}

/**************************************************************
  Function:dump_where_item
  Description:dump WHERE ... GROUP, column = 'S', table.column = table2.column do
        not allowed.
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_where_item()
{
    vector<string> tables;
    vector<string> where;
    vector<string>::iterator iter_table;
    vector<string>::iterator iter_where;
    string table_name;

    if (check_sqlstree_alive())
    {
        return false;
    }

    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "table size=%lu", tables.size());
        return false;
    }
    for (iter_table = tables.begin(); iter_table != tables.end(); iter_table++)
    {
        table_name = *iter_table;
        where = obselect->fetch_where_from_tree(*result_plan, table_name);
        for (iter_where = where.begin(); iter_where != where.end(); iter_where++)
        {
            query_store_where(table_name, *iter_where);
        }
    }
    return true;
}

/**************************************************************
  Function:dump_group_item 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_group_item()
{
    vector<string> tables;
    vector<GroupItem> groups;
    ushort next, next_table;

    if (check_sqlstree_alive())
    {
        return false;
    }

    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "table size=%lu", tables.size());
        return false;
    }
    for (next_table = 0; next_table < tables.size(); next_table++)
    {
        groups = obselect->fetch_group_from_tree(*result_plan, tables[next_table]);
        for (next = 0; next < groups.size(); next++)
        {
            query_store_group(tables[next_table], groups[next].group_column_, next,
                    groups[next].group_type_);
        }
    }
    return true;
}

/**************************************************************
  Function: dump_order_item
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_order_item()
{
    vector<string> tables;
    vector<OrderItem> orders;
    vector<OrderItem>::iterator iter_order;
    ushort next, next_order;

    if (check_sqlstree_alive())
    {
        return false;
    }

    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "table size=%lu", tables.size());
        return false;
    }
    for (next = 0; next < tables.size(); next++)
    {
        orders = obselect->fetch_order_from_tree(*result_plan, tables[next]);
        for (next_order = 0; next_order < orders.size(); next_order++)
        {
            query_store_order(tables[next], iter_order[next_order].order_column,
                    next_order, iter_order[next_order].order_type_);
        }
    }
    return true;
}

/**************************************************************
  Function:dump_limit_item 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_limit_item()
{
    LimitItem limit;
    limit = obselect->fetch_limit_from_tree(*this->result_plan);
    this->query_store_limit(limit.start, limit.end, 0);
    return true;
}

/**************************************************************
  Function: dump_having_item
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_having_item()
{
    vector<string> tables;
    if (check_sqlstree_alive())
    {
        return false;
    }

    tables = obselect->fetch_tables_from_tree(*result_plan);
    return true;
}

/**************************************************************
  Function: dump_syntax_tree
  Description: dump sql element from syntax tree.
  Author:
  Date:
  Input:
  Output:
  Return: dump success return true, otherwise return false.
  Other:
 **************************************************************/
bool
object_select::dump_syntax_tree()
{
    vector<string> tables;
    tables = obselect->fetch_tables_from_tree(*result_plan);
    if (tables.size() == 0)
    {
        jlog(WARNING, "object_select::dump_syntax_tree, query no table");
        return false;
    }

    jlog(INFO, "have_capabilities:%lu", have_capabilities);
    return dump_select_item() && dump_table_item() && dump_where_item() &&
            dump_group_item() && dump_order_item() && dump_limit_item() &&
            dump_having_item();
}
