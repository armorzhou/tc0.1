/* 
 * File:   cs_db.h
 * Author: tangchao
 *
 * Created on 2013年9月9日, 下午4:02
 */

#ifndef CS_DB_H
#define	CS_DB_H
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

#ifndef CPPTYPE_H
#include "../cpptype.h"
#endif
#ifndef CS_TABLE_H
#include "cs_table.h"
#endif

using namespace std;

class cs_db
{
public:

    cs_db()
    {

    }

    cs_db(const cs_db& orig)
    {

    }

    ~cs_db()
    {

    }

private:

public:
    map<string_db_name, class cs_table> db_map;
};

#endif	/* CS_DB_H */
