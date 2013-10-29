/* 
 * File:   cs_table.cpp
 * Author: tangchao
 * 
 * Created on 2013年9月9日, 下午4:03
 */

#include "cs_table.h"

/***************************************************************************/
/* class cs_table */

/***************************************************************************/

/**************************************************************
  Function:     set_db_name
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
void
cs_table::set_db_name(string db_name)
{
    this->db_name.assign(db_name);
    return;
}

/**************************************************************
  Function:     get_db_name
  Description:   
  Author:
  Date:
  Input:
  Output:
  Return: 
  Other:
 **************************************************************/
string
cs_table::get_db_name()
{
    return this->db_name;
}


