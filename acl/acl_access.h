/* 
 * File:   acl_priv.h
 * Author: tangchao
 *
 * Created on 2013年9月6日, 下午6:26
 */

#ifndef ACL_PRIV_H
#define	ACL_PRIV_H
#include "acl_convert.h"

class ACL_ACCESS:public ACL_CONVERT
{
public:

    ACL_ACCESS(long long priv) : access(priv) { }

    ACL_ACCESS(const ACL_ACCESS& orig) : access(orig.access) {
 }

    ~ACL_ACCESS() {
 }
    bool set_privilege(long long flag);
    bool add_privilege(long long flag);
    bool del_privilege(long long flag);
    long long get_privilege();
private:
    long long access;
};


#endif	/* ACL_PRIV_H */

