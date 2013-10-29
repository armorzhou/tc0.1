/* 
 * File:   optimizer_mysql.cpp
 * Author: tangchao
 * 未完待续
 * Created on 2013年8月9日, 上午11:38
 */

#include <string>
#include <vector>

#include "../log/jlog.h"
#include "../route/meta_reader.h"
#include "../tc_global.h"
#include "op_select_to_mysql.h"

/**************************************************************
  Function:fix_order 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
op_make_select::fix_bug_limit_no_order(string db_name, string table_name)
{
    uint next;
    bool have_primary = false;

    if (this->order.size())
    {
        jlog(ERROR, "error call fix_bug_limit_no_order");
        return false;
    }

    string primary = meta->get_primary_key(db_name, table_name);
    if (!primary.size())
    {
        jlog(ERROR, "%s.%s have no primary",
                db_name.size() ? db_name.c_str() : NULL,
                table_name.size() ? table_name.c_str() : NULL);
        return false;
    }

    for (next = 0; next < this->select.size(); next++)
    {
        if (!primary.compare(this->select[next].field_name))
        {
            have_primary = true;
        }
    }

    if (!have_primary)
    {
        this->query_store_select(table_name, primary, T_INVALID);
    }
    
    this->query_store_order(table_name, primary, 0, OrderItem::ASC);
    
    return true;
}

/**************************************************************
  Function:fix_group 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
bool
op_make_select::fix_group(string table_name)
{
    if (!this->group.size())
    {
        jlog(ERROR, "error called fix_group()");
        return false;
    }

    if (this->have_capabilities & SELECT_WITH_GROUPBY)
    {
        string first_group = this->group[0].field_name;
        if (meta->is_sharding_key(this->db, table_name, first_group) ||
                meta->is_primary_key(this->db, table_name, first_group))
        {
            obselect->set_follow_group(false);
        }
        else
        {
            obselect->set_follow_group(true);
            if (this->have_capabilities & SELECT_WITH_AGG)
            {
                fix_aggregate(true);
            }
            else
            {
                fix_aggregate(false);
            }
        }

        if (!this->order.size())
        {
            if(!fix_bug_limit_no_order(this->db, table_name))
            {
                return false;
            }
        }
    }
    return true;
}

/**************************************************************
  Function:fix_aggregate 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
void
op_make_select::fix_aggregate(bool f)
{
    obselect->set_follow_agg(f);
    return;
}

/**************************************************************
  Function:fix_order 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
void
op_make_select::fix_order(string table_name)
{
    uint next, next_select;

    if (!this->order.size())
    {
        jlog(ERROR, "error called");
        return;
    }

    for (next = 0; next < this->order.size(); next++)
    {
        for (next_select = 0; next_select < this->select.size(); next_select++)
        {
            if (!this->order[next].field_name.compare(this->select[next_select].field_name))
            {
                break;
            }
            query_store_select(table_name, this->order[next].field_name, T_INVALID);
        }
    }

    return;
}

/**************************************************************
  Function:make_sql_simple 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
op_make_select::fix_limit(ulong limit)
{
    if (this->limit.size() == 1)
    {
        obselect->set_limit(limit);
    }
    else
    {
        jlog(ERROR, "more than one limit");
    }
    return;
}

/**************************************************************
  Function:fix_order 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
void
op_make_select::make_sql_with_select()
{

}

/**************************************************************
  Function:make_sql_with_join 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
void
op_make_select::make_sql_with_join()
{

}

/**************************************************************
  Function:make_sql_simple 
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
op_make_select::make_sql_simple()
{
    if (this->table.join_table.size() != 1)
    {
        jlog(ERROR, "this is not simple sql. %lu tables", this->table.join_table.size());
        return false;
    }

    obj_part = this->table.join_table[0].table_name;
 
    if (this->have_capabilities & SELECT_WITH_LIMIT && 
            !(this->have_capabilities & SELECT_WITH_ORDERBY))
    {
        if(!fix_bug_limit_no_order(this->db, obj_part))
        {
            return false;
        }
    }

    if (this->have_capabilities & SELECT_WITH_GROUPBY &&
            this->have_capabilities & SELECT_WITH_AGG)
    {
        if(!fix_group(obj_part))
        {
            return false;
        }
    }

    if (this->have_capabilities & SELECT_WITH_ORDERBY)
    {
        fix_order(obj_part);
    }

    if (this->have_capabilities & SELECT_WITH_LIMIT)
    {
        if (this->limit.size() == 1)
        {
            fix_limit(this->limit[0].end);
        }
    }

    return true;
}

/**************************************************************
  Function:fill_field 
  Description:fill field to SELECT list.
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
op_make_select::fill_field(string *select_item, mysql_select *field)
{
    if(NULL == field || field->field_name.size() == 0)
    {
        jlog(ERROR, "field not exist");
        return;
    }
    
    switch (field->aggregate_func)
    {
        case T_FUN_MAX:
            select_item->append("MAX(");
            select_item->append(field->field_name.c_str());
            select_item->append(")");
            break;
        case T_FUN_MIN:
            select_item->append("MIN(");
            select_item->append(field->field_name.c_str());
            select_item->append(")");
            break;
        case T_FUN_SUM:
            select_item->append("SUM(");
            select_item->append(field->field_name.c_str());
            select_item->append(")");
            break;
        case T_FUN_COUNT:
            select_item->append("COUNT(");
            select_item->append(field->field_name.c_str());
            select_item->append(")");
            break;
        case T_FUN_AVG:
            select_item->append("AVG(");
            select_item->append(field->field_name.c_str());
            select_item->append(")");
            break;
        case T_INVALID:
            select_item->append(field->field_name.c_str());
            break;
        default:
            jlog(ERROR, "fill field unknown error.");
            break;
    }

    return;
}

void
op_make_select::fill_from(string *from)
{
    from->append(" FROM ");
    return;
}

/**************************************************************
  Function:fill_group 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
op_make_select::fill_group(string *group, string& field_name,
        GroupItem::GroupType sort)
{
    if (sort == GroupItem::ASC)
    {
        group->append(field_name.c_str());
        group->append(" ASC ");
    }

    if (sort == GroupItem::DESC)
    {
        group->append(field_name.c_str());
        group->append(" DESC ");
    }
    return;
}

/**************************************************************
  Function:fill_order 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
op_make_select::fill_order(string *order, string& field_name,
        OrderItem::OrderType sort)
{
    if (sort == OrderItem::ASC)
    {
        order->append(field_name.c_str());
        order->append(" ASC ");
    }
    if (sort == OrderItem::DESC)
    {
        order->append(field_name.c_str());
        order->append(" DESC ");
    }
    jlog(ERROR, "order by error");
    return;
}
/**************************************************************
  Function:fill_limit 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
op_make_select::fill_limit(string *limit, ulong start, ulong end)
{
    char buff[64];
    last_part.append(" LIMIT ");

    sprintf(buff, "%lu", start);
    limit->append(buff);
    sprintf(buff, "%lu", end);
    limit->append(",");
    limit->append(buff);
    return;
}

/**************************************************************
  Function:make_sql 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
bool
op_make_select::make_sql()
{
    uint next;

    for (next = 0; next < this->select.size(); next++)
    {
        if (next == 0)
        {
            first_part.assign("SELECT ");
        }
        if (next < this->select.size() && next != 0)
        {
            first_part.append(",");
        }
        fill_field(&first_part, &this->select[next]);
    }
    fill_from(&first_part);
    
    cout << "first:" << first_part << endl;
    printf("first:%s\n", first_part.c_str());
    for (next = 0; next < this->where.size(); next++)
    {
        if (next == 0)
        {
            last_part.assign(" WHERE ");
        }
        last_part.append(this->where[next].field_name.c_str());
    }

    for (next = 0; next < this->group.size(); next++)
    {
        if (next == 0)
        {
            last_part.append(" GROUP BY ");
        }
        if (next > 0 && next < this->group.size())
        {
            last_part.append(",");
        }
        fill_group(&last_part, this->group[next].field_name, this->group[next].sort);
    }

    /* no having sql */
    //for(next = 0; next < this->hav.size(); next++)
    for (next = 0; next < this->order.size(); next++)
    {
        if (next == 0)
        {
            last_part.append(" ORDER BY ");
        }
        if (next > 0 && next < this->order.size())
        {
            last_part.append(",");
        }
        fill_order(&last_part, this->order[next].field_name, this->order[next].sort);
    }

    if (this->have_capabilities & SELECT_WITH_LIMIT)
    {
        if (this->limit.size() == 1)
        {        
            if(this->limit[0].start + this->limit[0].end > 100000000)
            {
                jlog(WARNING, "limit page more than 100000000");
                return false;
            }
            fill_limit(&last_part, this->limit[0].start, this->limit[0].end);
        }
        else
        {
            jlog(ERROR, "not support more than one limit ");
            return false;
        }
    }

    return true;
}

/**************************************************************
  Function:fetch_query 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
int
op_make_select::fetch_query()
{
    make_sql();
    return 0;
}
/**************************************************************
  Function:fetch_query 
  Description:
  Author:
  Date:
  Input:string& physics_table, string *sql
  Output:string *sql
  Return:0, do onece query, > 0, do hierarchical query
  Other:
 **************************************************************/
int
op_make_select::fetch_query(string& physics_table, string *sql)
{
    make_sql();
    if(physics_table.size() == 0)
    {
        return -1;
    }
    sql->assign(this->first_part.c_str());
    sql->append(" ");
    sql->append(physics_table.c_str());
    
    if (this->last_part.size())
    {
        sql->append(" ");
        sql->append(this->last_part.c_str());
    }

    return 0;
}

/**************************************************************
  Function:SQLopt_init 
  Description:
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
op_select_to_mysql::SQLopt_init(class ObSelectStmt *select,
        ResultPlan& result_plan)
{
    bool select_simple = true;
    bool have_select = false;

    bool have_order = select->get_order_item_size() ? true : false;
    bool have_limit = select->has_limit();
    bool have_group = select->get_group_expr_size() ? true : false;
    bool have_join = select->get_joined_table_size() ? true : false;
    bool have_agg = select->get_agg_fun_size() ? true : false;

    jlog(INFO, "agg:%d, join:%d, group:%d, limit:%d, order:%d",
            have_agg, have_join, have_group, have_limit, have_order);
    form_capabilities = 0;
    if (select_simple)
        form_capabilities += SELECT_SINGLE_TABLE;
    if (have_order)
        form_capabilities += SELECT_WITH_ORDERBY;
    if (have_limit)
        form_capabilities += SELECT_WITH_LIMIT;
    if (have_group)
        form_capabilities += SELECT_WITH_GROUPBY;
    if (have_join)
        form_capabilities += SELECT_WITH_JOIN;
    if (have_select)
        form_capabilities += SELECT_WITH_SELECT;
    if (have_agg)
        form_capabilities += SELECT_WITH_AGG;

    init(form_capabilities, select, result_plan);
    jlog(INFO, "form_capabilities=%lu, have_capabilities=%lu",
            form_capabilities, have_capabilities);
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
op_select_to_mysql::SQLopt(class ObSelectStmt *select, ResultPlan& result_plan)
{
    SQLopt_init(select, result_plan);
    /* dump synatax tree to optimizer's instructure. sometimes we need rewrite sql.*/
    dump_syntax_tree();

    if (form_capabilities & SELECT_WITH_JOIN)
    {
        /*  */
        make_sql_with_join();
    }
    else if (form_capabilities & SELECT_WITH_SELECT)
    {
        /*  */
        make_sql_with_select();
    }
        /* Single table select without join and sub-select. */
    else
    {
        if(!make_sql_simple())
        {
            return false;
        }
    }
    
    /* simple sql single table */
    jlog(INFO, "\nsql:%s %s %s",
            first_part.size() ? first_part.c_str() : "",
            this->obj_part.size() ? obj_part.c_str() : "",
            last_part.size() ? last_part.c_str() : "");
    
    return true;
}

/**************************************************************
  Function:optimize
  Description:
  Author:
  Date:
  Input:
  Output:
  Return:
  Other:
 **************************************************************/
bool
SQLopt::optimize(class optimizer_mysql& arg, class ObSelectStmt *select,
        ResultPlan& result_plan)
{
    return arg.SQLopt(select, result_plan);
}
