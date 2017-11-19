/*
 *  FILE %ds/cal/dt001b.c
 *
 *  Ermittlung der Datums und der Zeit
 *
 *  written:       1989 03 18: extrahiert aus e:/xerox/cr.c
 *                 1994-07-18: extrahiert aus /usr/sbr/dt001.c
 *  latest update: 1998-07-19 10:04:20
 *
 */

#include <time.h>
#include <stdio.h>
#include <gg/dates.h>

#ifdef MSDOS
#pragma check_stack(off)
#endif

extern char *DAY_OF_WEEK3 [];
extern char *MONTH_NAME [];

/* ------------------------------------------------------------------------ */
int _actual_datim (
char *datum,    /* destination string for the formated date and time values */
int format,     /* see date_and_time_formats for more details               */
long tt)        /* actual time value to be formatted                        */
{
  struct tm *tm_s;
  int mon;
  int year;

  if (datum == (char *) 0 || tt < 0L) return -1;

  if (format & DATIMfmt_GMT)
  {
    tm_s= gmtime ((time_t *) &tt);
    format ^= DATIMfmt_GMT;
  }
  else
  {
    tm_s= localtime ((time_t *) &tt);
  }

  switch (format)
  {
    case DATIMfmt_none:
      datum [0]= 0;
      break;

    case DATIMfmt_date_time: /* 1989-03-18 14:04:40 */
      sprintf (datum, "%04d-%02d-%02d %2d:%02d:%02d",
               1900+tm_s->tm_year, tm_s->tm_mon+1, tm_s->tm_mday,
               tm_s->tm_hour, tm_s->tm_min,   tm_s->tm_sec);
      break;

    case DATIMfmt_date: /* 1989-03-18 */
      sprintf (datum, "%04d-%02d-%02d",
               1900+tm_s->tm_year, tm_s->tm_mon+1, tm_s->tm_mday);
      break;

    case DATIMfmt_this_month: /* 1989-03 */
    case DATIMfmt_next_month: /* 1989-03 */
    case DATIMfmt_last_month: /* 1989-03 */
      year= 1900+tm_s->tm_year;
      mon=  tm_s->tm_mon+1;

      switch (format)
      {
        case DATIMfmt_next_month:
          if (mon >= 12) { mon= 1; year++; }
          else mon++;
          break;
        case DATIMfmt_last_month:
          if (mon <= 1) { mon= 12; year--; }
          else mon--;
          break;
      }

      sprintf (datum, "%04d-%02d", year, mon);
      break;

    case DATIMfmt_dos_date: /* mm-dd-yyyy */
      sprintf (datum, "%02d-%02d-%04d",
               tm_s->tm_mon+1, tm_s->tm_mday, 1900+tm_s->tm_year);
      break;

    case DATIMfmt_time: /* 14:04:40 */
      sprintf (datum, "%2d:%02d:%02d",
               tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec);
      break;

    case DATIMfmt_time2: /* 04:04:40 */
      sprintf (datum, "%02d:%02d:%02d",
               tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec);
      break;

    case DATIMfmt_touch: /* mmddhhmmyy */
      sprintf (datum, "%02d%02d%02d%02d%02d",
               tm_s->tm_mon+1, tm_s->tm_mday,
               tm_s->tm_hour,  tm_s->tm_min, tm_s->tm_year);
      break;

    case DATIMfmt_touch_gnu: /* mm  dd  hh  mm ccyy.ss */
      sprintf (datum,  "%02d%02d%02d%02d%04d.%02d",
               tm_s->tm_mon+1, tm_s->tm_mday,
               tm_s->tm_hour,  tm_s->tm_min, tm_s->tm_year+1900,
               tm_s->tm_sec);
      break;

    case DATIMfmt_touch_aix: /*ccyy mm  dd  hh  mm . ss */
      sprintf (datum,  "%02d%02d%02d%02d%02d.%02d",
               tm_s->tm_year+1900,
               tm_s->tm_mon+1, tm_s->tm_mday,
               tm_s->tm_hour,  tm_s->tm_min,
               tm_s->tm_sec);
      break;

    case DATIMfmt_elm_mbox: /* Mon May 19 19:39:25 1994 */
      sprintf (datum, "%s %s %2d %2d:%02d:%02d %4d",
               DAY_OF_WEEK3 [tm_s->tm_wday%7],
               MONTH_NAME [(tm_s->tm_mon%12)+1],
               tm_s->tm_mday,
               tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec,
               tm_s->tm_year+1900);
      break;

    case DATIMfmt_seconds:
      sprintf (datum, "%ld", tt);
      break;
  }

  return 0;
}
