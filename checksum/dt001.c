/*
 *  FILE %ds/cal/dt001.c
 *
 *  Ermittlung der Datums und der Zeit
 *
 *  written:       1989 03 18: extrahiert aus e:/xerox/cr.c
 *                 1994-07-18: extrahiert in /usr/sbr/dt001.c
 *  latest update: 1997-04-29 10:30:11
 *
 */

#include <time.h>
#include <stdio.h>
#include <gg/dates.h>

#ifdef MSDOS
#pragma check_stack(off)
#endif

/* ------------------------------------------------------------------------ */
int actual_datim (
char *datum,    /* destination string for the formated date and time values */
int format)     /* see date_and_time_formats for more details               */
{
  return _actual_datim (datum, format, time ((time_t *) 0));
}
