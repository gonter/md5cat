/*
 *  FILE %ds/strings/str_0024.c
 *
 *  isolate words out of a string
 *
 *  written:       1993-10-27
 *  latest update: 1995-12-03
 *  $Id: str_0024.c,v 1.2 2005/09/04 13:59:15 gonter Exp $
 *
 */

#ifdef MSDOS
#pragma check_stack(off)
#endif

#include <sys/types.h>
#include <gg/strings.h>

/* ------------------------------------------------------------------------ */
int isolate_tokens (char *src, char *flds[], int n_flds)
{
  int ch;                       /* character under observation              */
  int cnt= 0;                   /* number of words isolated                 */
#define STA_noword      0       /* no word has been identified              */
#define STA_word        1       /* word has been identified                 */
  int sta= STA_noword;          /* status of the isolator automaton         */

  if (src == (char *) 0) return -1;

  for (; (ch= *src) != 0; src++)
  {
    switch (sta)
    {
      case STA_noword:
        if (!(ch == ' ' || ch == '\t'))
        {
          if (n_flds > 0)
          {
            flds [cnt]= src;
            n_flds--;
          }
          cnt++;
          sta= STA_word;
        }
        break;

      case STA_word:
        if (ch == ' ' || ch == '\t')
        {
          if (n_flds > 0) *src= 0;
          sta= STA_noword;
        }
        break;
    }
  }

  return cnt;
}
