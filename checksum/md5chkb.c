/*
 *  FILE ~/usr/contrib/md5/md5chkb.c
 *
 *  return a MD5 digest string for a given block
 *
 *  written:       1994-06-24
 *  latest update: 1995-08-06
 *
 */

#include <string.h>
#include <gg/dpp.h>
#include <contrib/global.h>
#include <contrib/md5.h>
#include <contrib/md5gg.h>

static char MD5_str [34];

/* ------------------------------------------------------------------------ */
char *MD5check_block (unsigned char *str, int lng)
{
  unsigned char *digest;

  digest= MD5chk2_block (str, lng);
  bin_to_hex ((char *) digest, MD5_str, 16);

  return MD5_str;
}
