/* 
 * File:   cs_table.h
 * Author: tangchao
 *
 * Created on 2013年9月9日, 下午4:03
 */

#ifndef CS_TABLE_H
#define	CS_TABLE_H
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

#ifndef CPPTYPE_H
#include "../cpptype.h"
#endif
#ifndef CS_FIELD_H
#include "cs_field.h"
#endif

using namespace std;

class cs_table
{
public:

    cs_table(string db_name) : db_name(db_name)
    {
    }

    cs_table(const cs_table& orig) : db_name(orig.db_name)
    {
    }

    ~cs_table()
    {
    }

private:
    string db_name;

public:
    map<string_table_name, class cs_field> table_map;
    void set_db_name(string db_name);
    string get_db_name();
};

#endif	/* CS_TABLE_H */

