/* 
 * File:   op_delete_to_mysql.h
 * Author: tangchao
 *
 * Created on 2013年9月29日, 下午6:16
 */

#ifndef OP_DELETE_TO_MYSQL_H
#define	OP_DELETE_TO_MYSQL_H

#ifndef OPTIMIZERMYSQL_H
#include "optimizer_mysql.h"
#endif

class op_delete_to_mysql : public op_mysql_delete {
public:
    op_delete_to_mysql();
    op_delete_to_mysql(const op_delete_to_mysql& orig);
    virtual ~op_delete_to_mysql();

    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) {

    }

    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) {
        return false;
    }
private:

};

#endif	/* OP_DELETE_TO_MYSQL_H */

