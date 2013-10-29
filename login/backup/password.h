/* 
 * File:   password.h
 * Author: tangchao
 *
 * Created on 2013年9月7日, 上午9:57
 */

#ifndef JOY_PASSWORD_H
#define	JOY_PASSWORD_H

void tc_create_random_string(char *to, unsigned int length, 
        struct rand_struct *rand_st);
bool tc_check_scramble(const unsigned char *scramble_arg, const char *message,
        const unsigned char *hash_stage2);


#endif	/* JOY_PASSWORD_H */

