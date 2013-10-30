/* 
 * File:   acl_priv.h
 * Author: tangchao
 *
 * Created on 2013年9月6日, 下午6:26
 */

#ifndef ACL_PRIV_H
#define	ACL_PRIV_H

class acl_priv
{
public:

    acl_priv(long long priv) : privilege(priv)
    {
    }

    acl_priv(const acl_priv& orig) : privilege(orig.privilege)
    {

    }

    ~acl_priv()
    {

    }

private:
    long long privilege;

public:
    bool set_privilege(long long flag);
    bool add_privilege(long long flag);
    bool del_privilege(long long flag);
    long long get_privilege();
};


#endif	/* ACL_PRIV_H */

