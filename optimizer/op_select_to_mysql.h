/* 
 * File:   op_select_to_mysql.h
 * Author: tangchao
 *
 * Created on 2013年9月29日, 下午6:06
 */

#ifndef OP_SELECT_TO_MYSQL_H
#define	OP_SELECT_TO_MYSQL_H

#ifndef OPTIMIZERMYSQL_H
#include "optimizer_mysql.h"
#endif
#ifndef OBJECT_SELECT_H
#include "object_select.h"
#endif

class op_make_select : public object_select {
public:
    void fix_limit(ulong limit);
    void fix_order(string table_name);
    bool fix_group(string table_name);
    void fix_aggregate(bool f);
    bool fix_bug_limit_no_order(string db_name, string table_name);

    void make_sql_with_join();
    void make_sql_with_select();
    bool make_sql_simple();

    void fill_field(string *select, mysql_select *field);
    void fill_from(string *from);
    void fill_group(string *group, string& field_name, GroupItem::GroupType sort);
    void fill_order(string *order, string& field_name, OrderItem::OrderType sort);
    void fill_limit(string *limit, ulong start, ulong end);
    int fetch_query();
    int fetch_query(string& physics_table, string *sql);
    bool make_sql();

public:
    string first_part;
    string last_part;
    string obj_part;
};

class op_select_to_mysql : public op_make_select, public optimizer_mysql {
public:

    op_select_to_mysql() {

    }

    op_select_to_mysql(const op_select_to_mysql& orig) {

    }

    virtual ~op_select_to_mysql() {

    }

    void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan);
    bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan);

private:
    ulong form_capabilities;
};

class SQLopt {
public:
    bool
    optimize(class optimizer_mysql& arg, class ObSelectStmt *select,
            ResultPlan& result_plan);
};
#endif	/* OP_SELECT_TO_MYSQL_H */
