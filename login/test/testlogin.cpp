/* 
 * File:   testlogin.cpp
 * Author: tangchao
 *
 * Created on 2013年10月25日, 下午3:40
 */

#include <cstdlib>
#include "../login55.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    SERVER_CONNECTION_CNF scc;
    PROTOCOL pl;
    LOGIN55 *lg = new LOGIN55();
    
    pl.init(1, 0400);
    lg->init(&scc, pl);

    return 0;
}
