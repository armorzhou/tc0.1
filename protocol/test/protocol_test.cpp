/* 
 * File:   protocol_test.cpp
 * Author: tangchao
 *
 * Created on 2013年10月24日, 下午2:17
 */

#include <cstdlib>
#include "../../listener/listener.h"
#include "../protocol_10.h"

using namespace std;

/*
 * 
 */
int
main(int argc, char** argv)
{
    int ip_sock;
    LISTENER *lr = new LISTENER();
    PROTOCOL *pl = new PROTOCOL_10();
    
    lr->init(&ip_sock, NULL, NULL);
    pl->init(ip_sock, 0400);
    if(!pl->net_send_ok(2, 100, 100, 100, "haha"))
    {
        cout << "send error\n";
    }
    else
    {
        cout << "send ok\n";
    }
    return 0;
}

