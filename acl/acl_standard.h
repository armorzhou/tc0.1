/* 
 * File:   acl_standard.h
 * Author: tangchao
 *
 * Created on 2013年9月30日, 下午5:26
 */

#ifndef ACL_STANDARD_H
#define	ACL_STANDARD_H
#include <sys/types.h>
#include <string>

using namespace std;

class acl_standard {
public:
    acl_standard();
    acl_standard(const acl_standard& orig);
    virtual ~acl_standard();
    
    virtual bool acl_check_field(string user, string db, string table, string field, uint p) = 0;
    virtual bool acl_check_field(string db, string table, string field, uint p) = 0;
    virtual bool acl_check_table(string db, string table, uint p) = 0;
    virtual bool acl_check_table(string user, string db, string table, uint p) = 0;
    virtual bool acl_check_db(string user, string db, uint p) = 0;
    virtual bool acl_check_db(string db, uint p) = 0;
    virtual bool acl_check_super(string user) = 0;
private:

};

#endif	/* ACL_STANDARD_H */

