/* 
 * File:   main.c
 * Author: tangchao
 *
 * Created on 2013年9月22日, 下午3:49
 */

#include <stdio.h>
#include <stdlib.h>
#include "../gvars.h"
#include "../init/init.h"
#include "listener.h"


/*
 * 
 */
int main(int argc, char** argv)
{
    unsigned int port = 3344;
    
    first_init();
    set_port(&port);

    listener_init();
    
    return (EXIT_SUCCESS);
}

