/* 
 * File:   optimizer.h
 * Author: tangchao
 *
 * Created on 2013年8月9日, 上午11:09
 */

#ifndef OPTIMIZER_H
#define	OPTIMIZER_H

#ifndef OCEANBASE_SQL_SELECTSTMT_H_
#include "../sql/ob_select_stmt.h"
#endif

using namespace std;
using namespace oceanbase::sql;

class optimizer {
public:
    virtual ~optimizer(){}
    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
private:

};

class op_insert {
public:
    virtual ~op_insert(){}
    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
private:

};
class op_update {
public:
    virtual ~op_update(){}
    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
private:

};

class op_delete {
public:
    virtual ~op_delete(){}
    virtual void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
    virtual bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan) = 0;
private:
};

#endif	/* OPTIMIZER_H */

