/* 
 * File:   optimizer_mysql.h
 * Author: tangchao
 *
 * Created on 2013年8月9日, 上午11:38
 */

#ifndef OPTIMIZERMYSQL_H
#define	OPTIMIZERMYSQL_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>

#include <sys/types.h>
#ifndef OPTIMIZER_H
#include "optimizer.h"
#endif
#ifndef OCEANBASE_SQL_SELECTSTMT_H_
#include "../sql/ob_select_stmt.h"
#endif
#include <string>

using namespace std;
using namespace oceanbase::sql;

class optimizer_mysql : public optimizer, public ObSelectStmt {
public:
    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan){
        
    }
    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan){
        return false;
    }
};

class op_mysql_delete : public op_delete {
public:

    virtual ~op_mysql_delete() {

    }

    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) {

    }

    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) {
        return false;
    }
};

class op_mysql_update : public op_update {
public:

    virtual ~op_mysql_update() {

    }

    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) {

    }

    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) {
        return false;
    }
};

class op_mysql_insert : public op_insert {
public:

    virtual ~op_mysql_insert() {

    }

    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) {

    }

    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) {
        return false;
    }
};

/* optimize */
bool optimize(class optimizer_mysql& arg, class ObSelectStmt *select, 
        ResultPlan& result_plan);
#endif	/* OPTIMIZERMYSQL_H */


