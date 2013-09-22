/* 
 * File:   main.c
 * Author: tangchao
 *
 * Created on 2013年9月22日, 下午12:00
 */

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    TCLOG("error", "welcome to here.");
    TCLOG("info", "kaka:%d", 1);
    return (EXIT_SUCCESS);
}

