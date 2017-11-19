/*
 *  FILE %sbr/bin2hex.c
 *
 *  convert an binary data block into a printable string
 *  see also:
 *  - hex2bin.c int cdecl hex_to_bin (char *hex, char *bin, int lng);
 *              convert a printable hex string into a binary data block
 *
 *  written:       1993-01-06: <Gerhard.Gonter@wu-wien.ac.at>
 *                 1995-02-20: renamed from md5d2s.c to bin2hex.c
 *  latest update: 1996-07-21 15:37:07
 *
 */

#include <gg/dpp.h>

static char hex_table [16]= "0123456789abcdef";

/* ------------------------------------------------------------------------ */
int bin_to_hex (char *bin, char *hex, int lng)
{
  int ch;
  int ch2;

  if (lng <= 0) return -1;

  while (lng-- > 0)
  {
    ch= *bin++ & 0x00FF;
    ch2= (ch >> 4) & 0x000F;
    ch &= 0x000F;

    *hex++= hex_table [ch2];
    *hex++= hex_table [ch];
  }
  *hex= 0;

  return 0;
}
