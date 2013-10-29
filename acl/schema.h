/* 
 * File:   ukey.h
 * Author: tangchao
 *
 * Created on 2013年9月6日, 下午3:02
 */

#ifndef UKEY_H
#define	UKEY_H
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>
#include <pthread.h>
#include <string>

#include <sys/types.h>

using namespace std;

class SCHEMA {
public:

    SCHEMA(string username, string host) : username(username), host(host) {
    }

    SCHEMA(const SCHEMA& orig) : username(orig.username), host(orig.host) {
    }

    virtual ~SCHEMA() {
    }

    bool operator<(const SCHEMA& orig) const {
        if (username < orig.username) {
            return true;
        } else {
            return false;
        }
    }

    ulong get_username_size() {
        return username.size();
    }

    ulong get_host_size() {
        return host.size();
    }

    void
    set_host(string host) {
        this->host = host;
        return;
    }

    string
    get_host() {
        return this->host;
    }

    void
    set_username(string username) {
        this->username = username;
        return;
    }

    string
    get_username() {
        return username;
    }
private:
    string username;
    string host;
};

#endif	/* UKEY_H */

