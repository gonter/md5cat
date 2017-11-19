/*
 *  FILE ~/usr/contrib/md5/md5chk2b.c
 *
 *  return a MD5 digest block for a given block
 *
 *  written:       1994-06-24
 *  latest update: 1995-05-10
 *
 */

#include <string.h>
#include <contrib/global.h>
#include <contrib/md5.h>
#include <contrib/md5gg.h>

static unsigned char digest [16]; /* message digest */

/* ------------------------------------------------------------------------ */
unsigned char *MD5chk2_block (unsigned char *str, int lng)
{
  MD5_CTX context;

  MD5Init (&context);
  MD5Update (&context, str, lng);
  MD5Final (digest, &context);

  return digest;
}
