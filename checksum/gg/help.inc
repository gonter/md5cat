/*
 *  include FILE <gg/help.inc>
 *
 *  Ausgabe von HELP-Klauseln
 *
 *  Voraussetzung:  static char *HELP []
 *                  int j;
 *                  STDHLP
 *
 *  written:       1988 02 25
 *                 1992 02 12: CMS
 *  latest update: 1995-08-21
 *
 */

#ifndef STDHLP
#define STDHLP  stdout
#endif

default :

#ifdef W_MODEL_CURSES
  endwin();
#endif

  for (j = 0; j < (sizeof (HELP) / sizeof (char *)); j++)
  {
    fprintf (STDHLP, HELP [j]);
#ifdef MSDOS
    if ((j % 23) == 22
        || (j+1 >= (sizeof (HELP) / sizeof (char *)) ))
      switch (getch ())
      {
        case    0: getch (); break;
        case  'b': case 'B': j = -1; break;
        case 0x1b: exit (0);
      }
#endif
    if (j+1 >= (sizeof (HELP) / sizeof (char *))) break;
  }
  exit (0);
