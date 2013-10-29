/* 
 * File:   object_select.h
 * Author: tangchao
 *
 * Created on 2013年9月30日, 下午2:13
 */

#ifndef OBJECT_SELECT_H
#define	OBJECT_SELECT_H

#include <vector>
#include <string>

#ifndef OPTIMIZERMYSQL_H
#include "optimizer_mysql.h"
#endif

#ifndef OCEANBASE_SQL_SELECTSTMT_H_
#include "../sql/ob_select_stmt.h"
#endif

/* select extension */
#define ALLOW_TABLES_IN_ONE_SQL                 1
/* select item */
#define SELECT_SINGLE_TABLE                     2 << 5      /* 2^5 */
#define SELECT_WITH_SELECT                      2 << 6      /* 2^6 */
#define SELECT_WITH_JOIN                        2 << 7     /* 2^7 */
#define SELECT_WITH_ORDERBY                     2 << 8     /* 2^8 */
#define SELECT_WITH_GROUPBY                     2 << 9     /* 2^9 */
#define SELECT_WITH_AGG                         2 << 10    /* 2^10 */
#define SELECT_WITH_HAVING                      2 << 11    /* 2^11 */
#define SELECT_WITH_WHERE                       2 << 12    /* 2^12 */
#define SELECT_WITH_LIMIT                       2 << 13    /* 2^13 */
#define SELECT_WITH_OR                          2 << 14
#define SELECT_WITH_IN                          2 << 15
#define SELECT_WITH_EXIST                       2 << 16
#define SELECT_WITH_BIND                        2 << 17
#define SELECT_WITH_RULE                        2 << 18
#define SELECT_WITH_SINGLE_DB                   2 << 19

using namespace std;

typedef struct st_mysql_order {
    string table_name;
    string field_name;
    ushort pos;
    OrderItem::OrderType sort;
} mysql_order;

typedef struct st_mysql_where 
{
    string table_name;
    string field_name;
    
} mysql_where;

typedef struct st_mysql_group 
{
    string table_name;
    string field_name;
    ushort pos;
    GroupItem::GroupType sort;
} mysql_group;

typedef struct st_mysql_select 
{
    string table_name;
    string field_name;
    SqlItemType aggregate_func;
} mysql_select;

typedef struct st_mysql_table {
    string db_name;
    string table_name;
    ushort pos;
} mysql_table;

typedef struct st_mysql_on
{
    string table_name;
    string field_name;
     /* left element record operator string. pos 0 opera_str 1[table.id = table.id]. */
    ushort pos;        
    string opera_str;
} mysql_on;

typedef struct st_mysql_join
{
    vector<mysql_table> join_table;
    string opera_str;
    bool have_join; /* true is select with join */
} mysql_join;

typedef struct st_mysql_hav
{
    string str;
} mysql_hav;

typedef struct st_mysql_limit
{
    ulong start;
    ulong end;
    ushort end_of_pos;
} mysql_limit;

class object_select {
public:
    void init(ulong capabilities, class ObSelectStmt *select, ResultPlan& resultplan) {
        have_capabilities = capabilities;
        obselect = select;
        result_plan = &resultplan;
        db.assign(resultplan.db_name);
        return;
    }

    ushort get_loop_times() {
        return have_loop;
    }

    bool is_query_loop() {
        return have_loop ? true : false;
    }

    void query_store_data(mysql_select& pre_select);
    void query_store_data(mysql_where& pre_where);
    void query_store_data(mysql_group& pre_group);
    void query_store_data(mysql_hav& pre_hav);
    void query_store_data(mysql_order& pre_order);
    void query_store_data(mysql_limit& pre_limit);
    void query_store_data(mysql_table& pre_join);
    void query_store_data(mysql_on& pre_on);
    void query_store_data(mysql_join& pre_on);

    void query_store_select(string table_name, string field_name, SqlItemType aggregate_func);
    void query_store_where(string table_name, string field_name);
    void query_store_group(string table_name, string field_name, ushort pos, GroupItem::GroupType sort);
    void query_store_hav(string str);
    void query_store_order(string table_name, string field_name, ushort pos, OrderItem::OrderType sort);
    void query_store_limit(ulong start, ulong end, ushort end_of_pos);
    void query_store_join(string db_name, string table_name, ushort pos, string opera_str, bool j);
    void query_store_on(string table_name, string field_name, ushort pos, string opera_str);
    
    bool check_sqlstree_alive();
    string fetch_physics_name_from_field(SelectItem *field);
    string fetch_physics_name_from_order(OrderItem *field);
    string fetch_physics_name_from_group(GroupItem *field);
    SqlItemType fetch_aggregate_func_from_field(SelectItem *field); 
    bool fetch_sort_from_order(OrderItem *field);

    void fix_bug_limit_no_order();
    
    void store_select_field(string table_name, SelectItem *field);
    
    bool dump_syntax_tree();
    bool dump_select_item();
    bool dump_table_item();
    bool dump_where_item();
    bool dump_group_item();
    bool dump_order_item();
    bool dump_limit_item();
    bool dump_having_item();

public:
    ulong have_capabilities;
    string db;
    //vector<mysql_table> table;
    ushort have_loop; /* execute more than one backend query times. */
    class ObSelectStmt *obselect;
    ResultPlan *result_plan;
    vector<mysql_select> select;
    vector<mysql_where> where;
    vector<mysql_group> group;
    vector<mysql_hav> hav;
    vector<mysql_order> order;
    vector<mysql_limit> limit;
    vector<mysql_on> on;
    mysql_join table;
};

#endif	/* OBJECT_SELECT_H */

