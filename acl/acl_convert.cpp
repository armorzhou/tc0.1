/* 
 * File:   acl_convert.cpp
 * Author: tangchao
 *
 * Created on 2013年8月15日, 下午3:05
 */

#include "../log/jlog.h"
#include "privileges.h"
#include "acl_convert.h"

/**************************************************************
  Function:     check_privilege
  Description:  checking privileges
  Author: 
  Date:         
  Input: privileges
  Output: 
  Return: check success return true, if a valid privilege return fasle
  Other:
 **************************************************************/
bool
ACL_CONVERT::check_privilege(long long flag)
{
    if (ALL_PRIVILEGES & flag)
    {
        return true;
    }

    jlog(ERROR, "invalid privilege %lld", flag);
    return false;
}

/**************************************************************
  Function:     get_string_pri
  Description:  convert privilege to string
  Author: 
  Date:         
  Input: 
  Output: 
  Return:
  Other:
 **************************************************************/
char*
ACL_CONVERT::get_string_pri(long long pri)
{
    if (pri == NONE_PRI)
    {
        return "NONE";
    }
    if (pri == SELECT_PRI)
    {
        return "SELECT";
    }
    if (pri == INSERT_PRI)
    {
        return "INSERT";
    }
    if (pri == UPDATE_PRI)
    {
        return "UPDATE";
    }
    if (pri == DELETE_PRI)
    {
        return "DELETE";
    }
    if (pri == TRUNCATE_PRI)
    {
        return "TRUNCATE";
    }
    if (pri == DROP_PRI)
    {
        return "DROP";
    }
    if (pri == CREATE_PRI)
    {
        return "CREATE";
    }
    if (pri == GRANT_PRI)
    {
        return "GRANT";
    }
    if (pri == REVOKE_PRI)
    {
        return "REVOKE";
    }
    if (pri == INIT_DB)
    {
        return "INIT";
    }
    return NULL;
}

/**************************************************************
  Function:     get_convert_pri
  Description:  convert privilege int ACL table to ACL key.
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
int
ACL_CONVERT::get_convert_pri(string pri)
{
    if (!pri.compare("NONE"))
    {
        return NONE_PRI;
    }
    if (!pri.compare("SELECT"))
    {
        return SELECT_PRI;
    }
    if (!pri.compare("INSERT"))
    {
        return INSERT_PRI;
    }
    if (!pri.compare("UPDATE"))
    {
        return UPDATE_PRI;
    }
    if (!pri.compare("DELETE"))
    {
        return DELETE_PRI;
    }
    if (!pri.compare("TRUNCATE"))
    {
        return TRUNCATE_PRI;
    }
    if (!pri.compare("DROP"))
    {
        return DROP_PRI;
    }
    if (!pri.compare("CREATE"))
    {
        return CREATE_PRI;
    }
    if (!pri.compare("GRANT"))
    {
        return GRANT_PRI;
    }
    if (!pri.compare("REVOKE"))
    {
        return REVOKE_PRI;
    }
    if (!pri.compare("INIT_DB"))
    {
        return INIT_DB;
    }
    jlog(ERROR, "invalid privilege %s", pri.size() ? pri.c_str() : NULL);
    return 0;
}

int
ACL_CONVERT::get_convert_pri(char* c_pri)
{
    string pri;
    pri.assign(c_pri);
    if (!pri.compare("NONE"))
    {
        return NONE_PRI;
    }
    if (!pri.compare("SELECT"))
    {
        return SELECT_PRI;
    }
    if (!pri.compare("INSERT"))
    {
        return INSERT_PRI;
    }
    if (!pri.compare("UPDATE"))
    {
        return UPDATE_PRI;
    }
    if (!pri.compare("DELETE"))
    {
        return DELETE_PRI;
    }
    if (!pri.compare("TRUNCATE"))
    {
        return TRUNCATE_PRI;
    }
    if (!pri.compare("DROP"))
    {
        return DROP_PRI;
    }
    if (!pri.compare("CREATE"))
    {
        return CREATE_PRI;
    }
    if (!pri.compare("GRANT"))
    {
        return GRANT_PRI;
    }
    if (!pri.compare("REVOKE"))
    {
        return REVOKE_PRI;
    }
    if (!pri.compare("INIT_DB"))
    {
        return INIT_DB;
    }
    jlog(ERROR, "invalid privilege %s", pri.size() ? pri.c_str() : NULL);
    return 0;
}
