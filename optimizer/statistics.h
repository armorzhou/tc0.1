/* 
 * File:   Statistics.h
 * Author: tangchao
 *
 * Created on 2013年8月9日, 上午11:36
 */

#ifndef STATISTICS_H
#define	STATISTICS_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

#ifndef CPPTYPE_H
#include "../cpptype.h"
#endif
#ifndef CS_DB_H
#include "cs_db.h"
#endif

using namespace std;

class statistics : public cs_db
{
public:

    statistics() : health(false)
    {
    }

    statistics(const statistics& orig)
    {
    }

    ~statistics()
    {
    }
    void close_statistics();
    void open_statistics();
    bool reset_statistics();
    bool reset();
    bool get_statistics_health();

    bool init_statistics();
    bool init_information_schema();
    bool init_table_status();

    void print_all();

    long long get_reckon_rows(string db_name, string table_name, string field_name);

    bool update_cardinality(string db_name, string table_name,
            string field_name, long long table_rows, long long cardinality);

    bool update_db(class cs_table *db, string table_name,
            string field_name, long long table_rows, long long cardinality);
    bool update_db(string db_name, string table_name,
            string field_name, long long table_rows, long long cardinality);

    bool update_table(class cs_table *db, class cs_field *table,
            string field_name, long long cardinality);
    bool update_table(class cs_table *db, string table_name,
            string field_name, long long table_rows, long long cardinality);

    bool add_db(string db_name);
    bool del_db(string db_name);
    cs_table* find_db(string db_name);

    bool add_table(string table_name, cs_table *table, long long table_rows);
    bool del_table(string table_name, cs_table *table);
    cs_field* find_table(string table_name, cs_table *table);

    bool add_field(string field_name, long long cardinality, cs_field *field);
    bool del_field(string field_name, class cs_field *field);
    bool find_field(string field_name, class cs_field *field);

    long long statistics_size();

    long long get_cardinality(string field_name, class cs_field *field);
    
private:
    bool health; //true open cbo, false close cbo
};

#endif	/* STATISTICS_H */

