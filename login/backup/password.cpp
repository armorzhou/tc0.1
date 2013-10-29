/* password checking routines */
/*****************************************************************************
  The main idea is that no password are sent between client & server on
  connection and that no password are saved in mysql in a decodable form.

  On connection a random string is generated and sent to the client.
  The client generates a new string with a random generator inited with
  the hash values from the password and the sent string.
  This 'check' string is sent to the server where it is compared with
  a string generated from the stored hash_value of the password and the
  random string.

  The password is saved (in user.password) by using the PASSWORD() function in
  mysql.

  This is .c file because it's used in libmysqlclient, which is entirely in C.
  (we need it to be portable to a variety of systems).
  Example:
    update user set password=PASSWORD("hello") where user="test"
  This saves a hashed number as a string in the password field.

  The new authentication is performed in following manner:

  SERVER:  public_seed=create_random_string()
           send(public_seed)

  CLIENT:  recv(public_seed)
           hash_stage1=sha1("password")
           hash_stage2=sha1(hash_stage1)
           reply=xor(hash_stage1, sha1(public_seed,hash_stage2)

           // this three steps are done in scramble() 

           send(reply)

     
  SERVER:  recv(reply)
           hash_stage1=xor(reply, sha1(public_seed,hash_stage2))
           candidate_hash2=sha1(hash_stage1)
           check(candidate_hash2==hash_stage2)

           // this three steps are done in check_scramble()

 *****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <mysql.h>
#include "../tc_global.h"
#include "../log/jlog.h"
#include "sha1.h"
#include "password.h"

/* Local functions */
static void tc_my_crypt(char *to, const unsigned char *s1, 
        const unsigned char *s2, unsigned int len);
static double tc_my_rnd(struct rand_struct *rand_st);
static bool tc_check_scramble_sha1(const unsigned char *scramble_arg, 
        const char *message, const unsigned char *hash_stage2);

/**************************************************************
  Function:     
  Description:    Generate random number.
  Author: 
  Date:         
  Input:   @param rand_st [INOUT] Structure used for number generation.
  Output: 
  Return:   @retval                Generated pseudo random number.
  Other:
 **************************************************************/
static double
tc_my_rnd(struct rand_struct *rand_st)
{
    rand_st->seed1 = (rand_st->seed1 * 3 + rand_st->seed2) % rand_st->max_value;
    rand_st->seed2 = (rand_st->seed1 + rand_st->seed2 + 33) % rand_st->max_value;
    return (((double) rand_st->seed1) / rand_st->max_value_dbl);
}

/*
 **************** MySQL 4.1.1 authentication routines *************
 */

/**************************************************************
  Function:     
  Description:  Generate string of printable pseudo random characters of requested length.
  
  @param to[out]    Buffer for generation; must be at least length+1 bytes
                    long; result string is always null-terminated
  @param length[in] How many random characters to put in buffer
  @param rand_st    Structure used for number generation
    
  @note This function is restricted for use with
    native_password_authenticate() because of security reasons.
      
  DON'T RELY ON THIS FUNCTION FOR A UNIFORMLY DISTRIBUTION OF BITS!
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
void
tc_create_random_string(char *to, unsigned int length, struct rand_struct *rand_st)
{
    char *end = to + length;
    /*
      Warning: my_rnd() is a fast prng, but it doesn't necessarily have a uniform
      distribution.
     */
    for (; to < end; to++)
        *to = (char) (tc_my_rnd(rand_st) * 94 + 33);
    *to = '\0';
}

/**************************************************************
  Function:     
  Description:      Encrypt/Decrypt function used for password encryption in authentication.
    Simple XOR is used here but it is OK as we crypt random strings. Note,
    that XOR(s1, XOR(s1, s2)) == s2, XOR(s1, s2) == XOR(s2, s1)
  SYNOPSIS
    my_crypt()
    to      OUT buffer to hold crypted string; must be at least len bytes
                long; to and s1 (or s2) may be the same.
    s1, s2  IN  input strings (of equal length)
    len     IN  length of s1 and s2
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
static void
tc_my_crypt(char *to, const unsigned char *s1, const unsigned char *s2, unsigned int len)
{
    const unsigned char *s1_end = s1 + len;
    while (s1 < s1_end)
        *to++ = *s1++ ^ *s2++;
}

/**************************************************************
  Function:tc_check_scramble_sha1     
  Description:  Check that scrambled message corresponds to the password; the function
    is used by server to check that recieved reply is authentic.
    This function does not check lengths of given strings: message must be
    null-terminated, reply and hash_stage2 must be at least SHA1_HASH_SIZE
    long (if not, something fishy is going on).
  SYNOPSIS
    check_scramble_sha1()
    scramble     clients' reply, presumably produced by scramble()
    message      original random string, previously sent to client
                 (presumably second argument of scramble()), must be 
                 exactly SCRAMBLE_LENGTH long and NULL-terminated.
    hash_stage2  hex2octet-decoded database entry
    All params are IN.
  Author: 
  Date:         
  Input: 
  Output: 
  Return:     true  password is correct
    false  password is invalid
  Other:
 **************************************************************/
static bool
tc_check_scramble_sha1(const unsigned char *scramble_arg, const char *message,
        const unsigned char *hash_stage2)
{
    unsigned char buf[SHA1_HASH_SIZE];
    unsigned char hash_stage2_reassured[SHA1_HASH_SIZE];
    bool res = false;

    jlog(INFO, "enter tc_check_scramble_sha1");
    
    /* create key to encrypt scramble */
    tc_compute_sha1_hash_multi(buf, message, SCRAMBLE_LENGTH,
            (const char *) hash_stage2, SHA1_HASH_SIZE);
    /* encrypt scramble */
    tc_my_crypt((char *) buf, buf, scramble_arg, SCRAMBLE_LENGTH);

    /* now buf supposedly contains hash_stage1: so we can get hash_stage2 */
    tc_compute_sha1_hash(hash_stage2_reassured, (const char *) buf, SHA1_HASH_SIZE);

    jlog(INFO, "message:%s, hash_stage2:%s, hash_stage2_reassured:%s", 
            message, hash_stage2, hash_stage2_reassured);
    
    res = memcmp(hash_stage2, hash_stage2_reassured, SHA1_HASH_SIZE) ? false : true;
    
    jlog(INFO, "tc_check_scramble_sha1 res:%d", res);
    return res;
}

/**************************************************************
  Function:     check_scramble
  Description: 
  Author: 
  Date:         
  Input: 
  Output: 
  Return: true check ok, false check failed.
  Other:
 **************************************************************/
bool
tc_check_scramble(const unsigned char *scramble_arg, const char *message,
        const unsigned char *hash_stage2)
{
    return tc_check_scramble_sha1(scramble_arg, message, hash_stage2);
}

#if 0  //for c++ string
/**************************************************************
  Function:     
  Description: 
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
static void
make_salt(unsigned char *to, const char *str)
{
    const char *str_end = str + SCRAMBLE_LENGTH + 1;
    while (str < str_end)
    {
        *to = *str;
        to++;
        str++;
    }
    return;
}
/**************************************************************
  Function:     tc_check_scramble
  Description: 
  Author: 
  Date:         
  Input: 
  Output: 
  Return: 
  Other:
 **************************************************************/
static bool
tc_check_scramble(const unsigned char *scramble_arg, const char *message,
        const char *hash_stage2)
{
    unsigned char hash_stage2_tmp[SCRAMBLE_LENGTH + 1];

    make_salt(hash_stage2_tmp, hash_stage2);
    jlog(INFO, "password:%s, hash_stage2:%s,  hash_stage2_tmp:%s", 
            scramble_arg, hash_stage2, hash_stage2_tmp);
    return check_scramble(scramble_arg, message, hash_stage2_tmp);
}
#endif
