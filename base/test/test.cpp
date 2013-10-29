/* 
 * File:   main.cpp
 * Author: tangchao
 *
 * Created on 2013年8月15日, 下午7:04
 */

#include <cstdlib>
#include "../cluster.h"

using namespace std;

/*
 * example
 */
int
main(int argc, char** argv)
{
    string product_name = ((argc > 1) ? argv[1] : "LanSe Joy");
    string version = ((argc > 2) ? argv[2] : "2.0.1");

    class cluster *instance = new cluster;

    instance->set_product_name(product_name);
    instance->set_version(version);

    cout << instance->get_product_name() << "-" << instance->get_version()
            << endl;

    delete instance;
    return 0;
}

