/* 
 * File:   cs_field.h
 * Author: tangchao
 *
 * Created on 2013年9月9日, 下午4:04
 */

#ifndef CS_FIELD_H
#define	CS_FIELD_H
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

#ifndef CPPTYPE_H
#include "../cpptype.h"
#endif

using namespace std;

class cs_field
{
public:

    cs_field(string table_name, long long table_rows) :
    table_name(table_name), table_rows(table_rows)
    {
    }

    cs_field(const cs_field& orig) :
    table_name(orig.table_name), table_rows(orig.table_rows)
    {
    }

    virtual ~cs_field()
    {
    }


private:
    string table_name;
    long long table_rows;

public:
    map<string_column_name, long long> field_map;

    void set_table_name(string table_name);
    string get_table_name();
    void set_table_rows(long long table_rows);
    long long get_table_rows();
};


#endif	/* CS_FIELD_H */

