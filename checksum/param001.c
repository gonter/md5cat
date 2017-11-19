/*
 *  FILE %sbr/param001.c
 *
 *  written:       1990 11 09
 *                 1993-04-06: cnt, renamed
 *  latest update: 1995-08-21
 *
 */

#include <gg/dpp.h>

#ifdef MSDOS
#pragma check_stack(off)
#endif

#define VARIANTE2

/* ------------------------------------------------------------------------ */
long _get_parameter_value (char *s, int cnt)
{
  long val      =  0L;
  long base     = 16L;
  int  lastchar =   6;
  int  signum   =   0;
  int  ch;

#define STAT_start      0       /* in the beginning ...                     */
#define STAT_oct_hex    1       /* decide between octal and hex             */
#define STAT_oct        2       /* octal number 0[0-7]                      */
#define STAT_hex        3       /* hexadecimal number 0[xX][0-9a-zA-Z]      */
                                /* or a number with any base:               */
                                /* [1-9][0-9]#[0-9a-zA-Z]                   */
#define STAT_dec        4       /* decimal number [1-9][0-9]                */
  int  stat= STAT_start;

  if (s == (char *) 0) return 0L;

  while ((ch= (int) (*s++ & 0x00FF)) != 0 && cnt-- > 0)
  {
    switch (stat)
    {
      case STAT_start:
        switch (ch)
        {
          case '0':
            stat= STAT_oct_hex;
            break;
#ifdef VARIANTE1
          case '+':
            signum= 0;
            break;
          case '-':
            signum= 1;
            break;
#endif
#ifdef VARIANTE2
          case '+':
            break;
          case '-':
            signum= (signum) ? 0 : 1;
            break;
#endif
          case ' ':
            break;
          default:
            if (ch < '1' || ch > '9') goto OVER;
            stat= STAT_dec;
            val= (long) (ch - '0');
            break;
        }
        break;

      case STAT_oct_hex:
        if (ch == '_') break;
        if (ch == 'x' || ch == 'X') stat= STAT_hex; else
        if (ch >= '0' && ch <= '7') 
        {
          stat= STAT_oct;
          val= (long) (ch - '0');
        }
        else goto OVER;
        break;

      case STAT_oct:
        if (ch == '_') break;
        if (ch < '0' || ch > '7') goto OVER;
        val= val*8L + ((long) (ch - '0'));
        break;

      case STAT_hex:
        if (ch == '_') break;
        if (ch >= '0' && ch <= '9')
        {
          val= val*base + ((long) (ch - '0'));
        } else
        if (ch >= 'A' && ch < ('A' + lastchar))
        {
          val= val*base + ((long) (ch - 'A' + 10));
        } else
        if (ch >= 'a' && ch < ('a' + lastchar))
        {
          val= val*base + ((long) (ch - 'a' + 10));
        } else goto OVER;
        break;

      case STAT_dec:
        if (ch == '_') break;
        if (ch < '0' || ch > '9') 
        {
          if (ch != '#') goto OVER;
          {
            stat= STAT_hex;
            base= val;
            val= 0L;
            if (base > 36L) goto OVER;
            lastchar= ((int) base) - 10;
          }
        } else val= val*10L + ((long) (ch - '0'));
        break;

      default: goto OVER;
    }
  }

OVER:
  return (signum) ? -val : val;
}

/* ------------------------------------------------------------------------ */
#ifdef TEST
static char line [1024];

main ()
{
  long rv;

  for (;;)
  {
    printf ("parameter: ");
    scanf  ("%s", line);
    rv= _get_parameter_value (line, 1024);
    printf ("value    : %ld\n", rv);
  }
}
#endif
