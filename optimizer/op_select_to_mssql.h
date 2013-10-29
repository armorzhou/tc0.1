/* 
 * File:   op_select_to_mssql.h
 * Author: tangchao
 *
 * Created on 2013年9月29日, 下午6:10
 */

#ifndef OP_SELECT_TO_MSSQL_H
#define	OP_SELECT_TO_MSSQL_H

#ifndef OPTIMIZERMYSQL_H
#include "optimizer_mysql.h"
#endif

class op_select_to_mssql:public optimizer_mysql {
public:
    op_select_to_mssql(){
        
    }
    op_select_to_mssql(const op_select_to_mssql& orig){
        
    }
    virtual ~op_select_to_mssql(){
        
    }
    void SQLopt_init(class ObSelectStmt *select, ResultPlan& result_plan)
    {
        
    }
    bool SQLopt(class ObSelectStmt *select, ResultPlan& result_plan){
        return false;
    }
private:
    ulong form_capabilities;

};

#endif	/* OP_SELECT_TO_MSSQL_H */

