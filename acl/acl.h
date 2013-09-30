/* 
 * File:   acl.h
 * Author: tangchao
 *
 * Created on 2013年9月30日, 下午5:25
 */

#ifndef ACL_H
#define	ACL_H

#include <sys/types.h>
#include <string>

#ifndef ACL_STANDARD_H
#include "acl_standard.h"
#endif

using namespace std;

class acl : public acl_standard {
public:
    acl();
    acl(const acl& orig);
    virtual ~acl();

    virtual bool acl_check_field(string user, string db, string table, string field, uint p);
    virtual bool acl_check_field(string db, string table, string field, uint p);
    virtual bool acl_check_table(string db, string table, uint p);
    virtual bool acl_check_table(string user, string db, string table, uint p);
    virtual bool acl_check_db(string user, string db, uint p);
    virtual bool acl_check_db(string db, uint p);
    virtual bool acl_check_super(string user);
    
private:
};

#endif	/* ACL_H */

