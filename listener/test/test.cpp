/* 
 * File:   test.cpp
 * Author: tangchao
 *
 * Created on 2013年10月24日, 下午2:35
 */

#include <cstdlib>
#include "../listener.h"

using namespace std;

/*
 * 
 */
int
main(int argc, char** argv)
{
    LISTENER *lr = new LISTENER();
    int ip_sock;
    lr->init(&ip_sock, NULL, NULL);

    return 0;
}

