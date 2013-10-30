/* 
 * File:   acl_access.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月6日, 下午6:26
 */

#include "acl_access.h"
#include "acl_convert.h"

/**************************************************************
  Function:     set_privilege
  Description:  set a privileges instead of old privileges
  Author: 
  Date:         
  Input: privileges
  Output: 
  Return: set success return true, otherwise return fasle
  Other:
 **************************************************************/
bool
ACL_ACCESS::set_privilege(long long flag)
{
    if (check_privilege(flag))
    {
        access = flag;
        return true;
    }
    else
    {
        return false;
    }
}

/**************************************************************
  Function:     add_privilege
  Description:  add new privileges
  Author: 
  Date:         
  Input: privileges
  Output: 
  Return: add success return true, otherwise return fasle
  Other:
 **************************************************************/
bool
ACL_ACCESS::add_privilege(long long flag)
{
    if (check_privilege(flag))
    {
        access += (flag - (access & flag));
        return true;
    }
    return false;
}

/**************************************************************
  Function:     del_privilege
  Description:  delete a privileges from old privileges
  Author: 
  Date:         
  Input: privileges
  Output: 
  Return: delete success return true, otherwise return fasle
  Other:
 **************************************************************/
bool
ACL_ACCESS::del_privilege(long long flag)
{
    if (check_privilege(flag))
    {
        access -= flag;
        return true;
    }
    else
    {
        return false;
    }
}

/**************************************************************
  Function:     get_privilege
  Description:  get current privileges
  Author: 
  Date:         
  Input: 
  Output: 
  Return: privileges
  Other:
 **************************************************************/
long long
ACL_ACCESS::get_privilege()
{
    return access;
}
