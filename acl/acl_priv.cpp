/* 
 * File:   acl_priv.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月6日, 下午6:26
 */
#include "acl_priv.h"
#include "acl_convert.h"


/***************************************************************************/
/* class acl_priv 俗称 权限列表，用户的每个资源都有一个权限列表 */

/***************************************************************************/

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
acl_priv::set_privilege(long long flag)
{
    if(check_privilege(flag))
    {
        /* 避免权限重叠造成BUG，已经有的权限不需要重复增加 */
        this->privilege = flag;
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
acl_priv::add_privilege(long long flag)
{
    if(check_privilege(flag))
    {
        /* 避免权限重叠造成BUG，已经有的权限不需要重复增加 */
        this->privilege += (flag - (this->privilege & flag));
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
acl_priv::del_privilege(long long flag)
{
    /* 删除一个权限，权限必须符合规则，否则不能删除 */
    if(check_privilege(flag))
    {
        this->privilege -= flag;
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
acl_priv::get_privilege()
{
    return this->privilege;
}
