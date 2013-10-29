/* 
 * File:   op_insert_to_mysql.h
 * Author: tangchao
 *
 * Created on 2013年9月29日, 下午6:16
 */

#ifndef OP_INSERT_TO_MYSQL_H
#define	OP_INSERT_TO_MYSQL_H

#ifndef OPTIMIZERMYSQL_H
#include "optimizer_mysql.h"
#endif

class op_insert_to_mysql : public op_mysql_insert {
public:
    op_insert_to_mysql();
    op_insert_to_mysql(const op_insert_to_mysql& orig);
    virtual ~op_insert_to_mysql();

    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) {

    }

    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) {
        return false;
    }
};

#endif	/* OP_INSERT_TO_MYSQL_H */

