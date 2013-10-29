/* 
 * File:   tc_sha1.h
 * Author: tangchao
 *
 * Created on 2013年9月11日, 上午11:42
 */

#ifndef JOY_SHA1_H
#define	JOY_SHA1_H

#include <openssl/sha.h>
#ifndef TC_GLOBAL_H
#include "../tc_global.h"
#endif

void tc_compute_sha1_hash_multi(unsigned char *digest, const char *buf1, int len1,
                             const char *buf2, int len2);
void tc_compute_sha1_hash(unsigned char *digest, const char *buf, int len);
int tc_mysql_sha1_result(SHA_CTX *context, unsigned char Message_Digest[SHA1_HASH_SIZE]);
int tc_mysql_sha1_input(SHA_CTX *context, const unsigned char *message_array,
                     unsigned length);
int tc_mysql_sha1_reset(SHA_CTX *context);

#endif	/* JOY_SHA1_H */

