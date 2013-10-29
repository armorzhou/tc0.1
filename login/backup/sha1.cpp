#include "sha1.h"

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
int
tc_mysql_sha1_reset(SHA_CTX *context)
{
    return SHA1_Init(context);
}

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
int
tc_mysql_sha1_input(SHA_CTX *context, const unsigned char *message_array,
        unsigned length)
{
    return SHA1_Update(context, message_array, length);
}

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
int
tc_mysql_sha1_result(SHA_CTX *context,
        unsigned char Message_Digest[SHA1_HASH_SIZE])
{
    return SHA1_Final(Message_Digest, context);
}

/**************************************************************
  Function:     
  Description: Wrapper function to compute SHA1 message digest.        
  Author: 
  Date:         
  Input: buf            Message to be computed
        len       Length of the message   
  Output: digest        Computed SHA1 digest
  Return: void
  Other:
 **************************************************************/

void
tc_compute_sha1_hash(unsigned char *digest, const char *buf, int len)
{
    SHA_CTX sha1_context;

    tc_mysql_sha1_reset(&sha1_context);
    tc_mysql_sha1_input(&sha1_context, (const unsigned char *) buf, len);
    tc_mysql_sha1_result(&sha1_context, digest);
    return;
}

/**************************************************************
  Function:     tc_compute_sha1_hash_multi
  Description:   Wrapper function to compute SHA1 message digest for
  two messages in order to emulate sha1(msg1, msg2).        
  Author: 
  Date:         
  Input:  @param buf1   [in]   First message
  @param len1   [in]   Length of first message
  @param buf2   [in]   Second message
  @param len2   [in]   Length of second message
  Output:   @param digest [out]  Computed SHA1 digest
  Return:   @return              void
  Other:
 **************************************************************/
void
tc_compute_sha1_hash_multi(unsigned char *digest, const char *buf1, int len1,
        const char *buf2, int len2)
{
    SHA_CTX sha1_context;

    tc_mysql_sha1_reset(&sha1_context);
    tc_mysql_sha1_input(&sha1_context, (const unsigned char *) buf1, len1);
    tc_mysql_sha1_input(&sha1_context, (const unsigned char *) buf2, len2);
    tc_mysql_sha1_result(&sha1_context, digest);
    return;
}

