/*
 *  include File <contrib/md5gg.h>
 *
 *  GG's utility functions for MD5 manipulation
 *
 *  written:       1993-01-05: <Gerhard.Gonter@wu-wien.ac.at>
 *  latest update: 1995-09-01
 *
 */

#ifndef __contrib_md5gg__
#define __contrib_md5gg__

#include <gg/floskel.h>

#ifdef __GG_dpp__
/* if <gg/dpp.h> is included, there are also these routines: */
# define MD5Digest2String(digest,string) bin_to_hex((digest),(string),16)
# define MD5String2Digest(digest,string) hex_to_bin((string),(digest),16)
#else
int cdecl MD5Digest2String (char *digest, char *string);
int cdecl MD5String2Digest (char *digest, char *string);
#endif

char *cdecl MD5check_string (unsigned char *str);
char *cdecl MD5check_block (unsigned char *str, int lng);
unsigned char *cdecl MD5chk2_block (unsigned char *str, int lng);
unsigned char *cdecl MD5chk2_string (unsigned char *str);
unsigned char *cdecl MD5chk2_block_array (unsigned char *str [],
  int lng [], int cnt);

#ifdef _FILE_DEFINED
unsigned char *cdecl MD5chk_stream (FILE *fi, long size);
#endif /* _FILE_DEFINED */
unsigned char *MD5chk_file_block (char *fnm, long beg, long size);
#define MD5chk_file(fnm) MD5chk_file_block((fnm), 0L, 0x7FFFFFFFL);

#endif /* __contrib_md5gg__ */
