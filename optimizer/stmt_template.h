/* 
 * File:   stmt_template.h
 * Author: tangchao
 *
 * Created on 2013年8月16日, 上午10:51
 */

#ifndef STMT_TEMPLATE_H
#define	STMT_TEMPLATE_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <list>

#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif
#include "../base/sql_column.h"
#include "../base/sql_table.h"
#include "../base/sql_db.h"

class relation_algorithm {
public:

    relation_algorithm() {
    }

    relation_algorithm(const relation_algorithm& orig) {
    }

    virtual ~relation_algorithm() {
    }

private:
    enum relation_algor algor;

public:
    void set_algor(enum relation_algor algor);
    enum relation_algor get_algor();
};

class relation_table {
public:

    relation_table() {
    }

    relation_table(const relation_table& orig) {
    }

    virtual ~relation_table() {
    }
    
private:
    string table_name;
    string column_name;

public:
    void set_table_name(string table_name);
    void set_column_name(string column_name);
    string get_table_name();
    string get_column_name();
};

class relation_template {
public:

    relation_template() {
    }

    relation_template(const relation_template& orig) {
    }

    virtual ~relation_template() {
    }

private:

public:
    list<class relation_table> tables;
    list<class relation_algorithm> algor;


};

class group_template {
public:

    group_template() {
    }

    group_template(const group_template& orig) {
    }

    virtual ~group_template() {
    }

private:

};

class order_template {
public:

    order_template() {

    }

    order_template(const order_template& orig) {

    }

    virtual ~order_template() {

    }
};

class column_template : public sql_column {
public:

    column_template() {
    }

    column_template(const column_template& orig) {
    }

    virtual ~column_template() {
    }

private:
    enum aggregate_func aggregate;
    string column_value;

public:
    void set_column_value(string column_value);
    string get_column_value();
};

class stmt_template {
public:

    stmt_template() {
    }

    stmt_template(const stmt_template& orig) {
    }

    ~stmt_template() {
    }


private:
    list<string> table;
    enum JOIN_TYPE join;
    enum RELATIONS relation;
    list<class column_template> where;
    list<class column_template> select;
    list<class group_template> group;
    list<class order_template> order;
    int start, offset;

public:
    class stmt_template* get_select(string table_name);
    void set_join(enum JOIN_TYPE join);
    enum JOIN_TYPE get_join();

};

#endif	/* STMT_TEMPLATE_H */
