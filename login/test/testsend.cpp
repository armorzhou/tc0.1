/* 
 * File:   testsend.cpp
 * Author: tangchao
 *
 * Created on 2013年10月25日, 下午4:07
 */

#include <cstdlib>
#include "../send_server_handshake.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    PROTOCOL pl;
    pl.init(100, 0400);
    
    SEND_SERVER_HANDSHAKE *ssh = new SEND_SERVER_HANDSHAKE();
    ssh->init(pl, 10, "5.5.14", 6, "mysql_native_password", 21, "sssssssss", 21);
    
    delete ssh;
    delete pl;
    return 0;
}

