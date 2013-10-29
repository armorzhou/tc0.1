/* 
 * File:   cs_field.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月9日, 下午4:04
 */

#include "cs_field.h"

/***************************************************************************/
/* class cs_field */

/***************************************************************************/

/**************************************************************
  Function:     set_table_name
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
cs_field::set_table_name(string table_name)
{
    this->table_name.assign(table_name);
    return;
}

/**************************************************************
  Function:     get_table_name
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
string
cs_field::get_table_name()
{
    return this->table_name;
}

/**************************************************************
  Function:     set_table_rows
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
cs_field::set_table_rows(long long table_rows)
{
    this->table_rows = table_rows;
    return;
}

/**************************************************************
  Function:     get_table_rows
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
long long
cs_field::get_table_rows()
{
    return this->table_rows;
}
