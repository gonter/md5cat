/*
 *  FILE ~/usr/contrib/md5/md5chks.c
 *
 *  return the md5 digest string for a given input string
 *
 *  written:       1994-06-24
 *  latest update: 1995-08-06
 *
 */

#include <string.h>
#include <contrib/md5gg.h>

/* ------------------------------------------------------------------------ */
char *MD5check_string (unsigned char *str)
{
  return MD5check_block (str, strlen ((char *) str));
}
