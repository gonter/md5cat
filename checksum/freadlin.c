/*
 *  FILE %sbr/freadlin.c
 *
 *  read exactly one line, nothing more, nothing less.
 *  take care of the buffer and discard additional characters
 *  and carriage returns and line feeds
 *
 *  written:       1989 03 18: extracted from /usr/ned/testprnt.c
 *                 1990 10 13: revision
 *                 1993-02-02: small revision
 *  latest update: 1997-11-02 17:42:19
 *
 */

#include <stdio.h>
#include <gg/sbr.h>

#ifdef MSDOS
#pragma check_stack(off)
#endif

#define EOLN 0x000A
#define IGNORE_CR

#ifdef MAC_MODE
#define fread_line fread_mac_line
#undef EOLN
#define EOLN 0x000D
#undef IGNORE_CR
#endif

#ifdef __CMS__
#undef EOLN
#define EOLN '\n'
#undef IGNORE_CR
#endif

/* ------------------------------------------------------------------------ */
int fread_line (
FILE *fi,                       /* file to read from                        */
char *p,                        /* line buffer                              */
int   c)                        /* size of line buffer                      */
/* RETURNS: < 0 ... error or END-OF-FILE                                    */
/*          = 0 ... empty line                                              */
/*          > 0 ... number of characters read into line buffer              */
/* REMARKS: if c is smaller than the length of line, then the               */
/*          remaining characters in the line are read away.                 */
/*          RETURN-characters (0x0D) are ignored                            */
{
  int k;                        /* character read from the input file       */
  int r= 0;                     /* number of the character in the buffer    */

  if (p == (char *) 0) c= 0;    /* no buffer, just read over line!          */
  else p[0]= 0;                 /* clear the buffer, for if it's EOF        */
  if (feof (fi)) return -1;     /* file at EOF; report this ...             */

  c--;                          /* the last character will be 0x00          */

  for (;;)
  {
    k= fgetc (fi) & 0x00FF;     /* we treat the character as unsigned ...   */
                                /* ... as it should be!                     */
    if (feof (fi))   break;     /* nothing more to read, this is *no* error */

#ifdef IGNORE_CR
    if (k == 0x000D) continue;  /* Carriage Return is ignored               */
#endif /* IGNORE_CR */

    if (k == EOLN) break;       /* Line Feed is *our* end-of-line           */

    if (c <= 0)      continue;  /* read away characters past the buffer     */
    *p++ = (char) k;
    c--;
    r++;
  }

  if (r >= 0) *p= 0x00;         /* terminate the buffer ...                 */
  return r;                     /* ... and report, how many characters      */
                                /* are in the buffer                        */
}
