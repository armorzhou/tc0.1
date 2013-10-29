/* 
 * File:   acl_convert.h
 * Author: tangchao
 *
 * Created on 2013年9月6日, 下午6:29
 */

#ifndef ACL_CONVERT_H
#define	ACL_CONVERT_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

using namespace std;

class ACL_CONVERT {
public:
    bool check_privilege(long long flag);
    int get_convert_pri(string pri);
    int get_convert_pri(char* c_pri);
    char* get_string_pri(long long pri);
};
#endif	/* ACL_CONVERT_H */

