/*
 *  FILE %usr/util/checksum.c
 *
 *  Ermitteln und ueberpruefen von Checksummen fuer Files
 *  -  prototypes
 *  -  HELP
 *
 *  written:       1987 12 02
 *  latest update: 1999-04-17 11:29:34
 *  $Id: checksum.c,v 1.5 2015/10/15 04:56:38 gonter Exp $
 *
 */

#ifndef MSDOS
#ifndef cdecl
#define cdecl
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef MSDOS
#include <conio.h>
#include <gg/filename.h>
#endif
#include <gg/dates.h>
#include <gg/sbr.h>
#include <gg/strings.h>
#include <gg/dpp.h>

#define ALLOW_MD5
#ifdef ALLOW_MD5
#include <string.h>
#include <contrib/global.h>
#include <contrib/md5.h>
#include <contrib/md5gg.h>
#endif

#define TMP "tmptmp.tmp"

/* Quick Hack: ------------------------------------------------------------ */
#ifdef MSDOS
#define SUBDIRS                         /* Rekusive Suche durch das         */
                                        /* Filesystem moeglich              */
#define BETTER_FILENAME_MATCH           /* eigene Match Funktion            */
                                        /* sonst DOS Match Funktion         */
#else
#define wcrd_wildcards main_fnc
#endif

/* ------------------------------------------------------------------------ */
static int verbose_mode= 0;
#define LINE_LENGTH 2048
static char line [LINE_LENGTH];

#ifdef ALLOW_MD5
static int use_md5= 1;                  /* use MD5 finger prints            */
#define MD5_bufsiz 1024
static unsigned char MD5_buffer [MD5_bufsiz];
#endif

static int echo_mode= 0;                /* write everything to stderr too   */
static int disp_ok= 1;                  /* OK-Files anzeigen                */
static int disp_nf= 1;                  /* Nicht gefundene anzeigen         */

static long T_siz_files= 0L;            /* total size of all checked files  */
static long T_num_files= 0L;            /* total number of checked files    */

static int Comparemode= 0;              /* 1 -> compare files               */
static long Ncmp= 0L;                   /* number of compared files         */
static long Ngood= 0L;                  /* number of good files             */
static long Nbad= 0L;                   /* nubmer of bad files              */
static long Nfnd= 0L;                   /* number of files unfound files    */

static int badflag;

#define CHECKSUM_FORMAT_OLD 0
#define CHECKSUM_FORMAT_NEW 1
#define CHECKSUM_FORMAT_EXTENSIVE 2
static int xformat= CHECKSUM_FORMAT_NEW;

static char *NOT_OPENED= "***_not_opened_***";

/* ------------------------------------------------------------------------ */
static char *HELP [] =
{
  "USAGE: checksum [<options>] (<filename>)*\n",
  "Options:\n",
  "  -a ... display only accessible files (compare mode)\n",
  "  -b ... \"       \"    bad files (compare mode)\n",
  "  -c ... comparemode; -f<fnm> required!\n",
  "  -e ... echo mode: copy output to stderr\n",
  "  -f<fnm> ... list of files to check in <filename>\n",
  "  -q<fnm> ... same as -a -b -c -f<filename>\n",
  "  -x<n>   ... experimental format, 0..normal, 1..md5 directory\n",
  "  -s<str> ... generate the checksum for a string\n",
  "  -t      ... do NOT print total statistics\n",
#ifdef SUBDIRS
  "  -r ... subdirs\n",
#endif
#ifdef ALLOW_MD5
  "  -md5 ... use MD5 finger printing algorithm [DEF]\n",
  "           \"RSA Data Security, Inc. MD5 Message-Digest Algorithm\"\n",
  "           This algorithm was specified in rfc1321.\n",
  "  -nomd5 ... don't use MD5 finger printing algorithm\n",
#endif
  "EXAMPLES:  \n",
  "  checksum -e -ffiles.chk >files.cks ... take  \'finger prints\'\n",
  "  checksum -c -ffiles.cks            ... check \'finger prints\'\n",
  "\n",
  "(@)Achecksum 1.25 #D$1999-04-17 11:29:45\n",
  "\n",
#include <gg/public.inc>
} ;

/* prototypes: ------------------------------------------------------------ */
int cdecl main (int argc, char *argv []);
int cdecl main_fnc (char *fnm);
char *cdecl CMPl (long l1, long l2);
char *cdecl CMPi (int l1, int l2);
int cdecl check_file (char *fnm, long c1, long c2, long c3, int c4,
  char *xdigest_string);

/* ------------------------------------------------------------------------ */
int main (int argc, char *argv [])
{
  int i, j, cmps;
  FILE *fi2;
  int fi2_opened= 0;
  u_int64_t cnt1;
  long cnt2, cnt3;
  int cnt4;
  int rc;
  int print_summary= 1;
  int num_fields;
#define N_FIELDS 10
  char *fields [N_FIELDS];
  char *x_filenm;
  char *x_digest_string;
  char *time_start;
  char *time_end;
  char *arg;

  Comparemode= 0;

  actual_datim (line, DATIMfmt_date_time);
  time_start= strdup (line);

  for (i= 1; i < argc; i++)
  {
    arg= argv [i];
    if (arg [0] == '-')
    {
      switch (arg [1])
      {
        case 'q': case 'Q': 
          disp_nf= 0; 
          disp_ok= 0;
          Comparemode= 1;

        case 'f': case 'F':
          if (arg [2] == 0) break;

          fi2= (FILE *) 0;
          if (strcmp (arg+2, "-") == 0)
          {
            fi2= stdin;
          }
          else
          {
            fi2= fopen (arg+2, "rt");
            fi2_opened= 1;
          }
          if (fi2 == (FILE *) 0) break;

          for (;;)
          {
            rc= fread_line (fi2, line, LINE_LENGTH);
            if (rc <= 0 && feof (fi2)) break;
            if (line [0] == '#' || line [0] == 0) continue;

            if (Comparemode)
            {
#ifdef ALLOW_MD5
              if (use_md5)
              {
                switch (xformat)
                {
                  case CHECKSUM_FORMAT_NEW:
                    num_fields= isolate_tokens (line, fields, 4);
                    x_digest_string=    fields [0];
                    /* x_type=          fields [1]; */
                    /* x_size=          fields [2]; */
                    x_filenm=           fields [3];
                    break;
                  default:
                    num_fields= isolate_tokens (line, fields, 2);
                    x_filenm=           fields [0];
                    x_digest_string=    fields [1];
                    break;
                }
              }
              else
#endif
              {
                num_fields= isolate_tokens (line, fields, 5);
                x_filenm= fields [0];
                sscanf   (fields [1], "%lx", &cnt1);
                sscanf   (fields [2], "%lx", &cnt2);
                sscanf   (fields [3], "%lx", &cnt3);
                sscanf   (fields [4], "%x",  &cnt4);
              }
            }
            else
            {
              x_filenm= line;
            }

            check_file (x_filenm, cnt1, cnt2, cnt3, cnt4, x_digest_string);
          }

          if (fi2_opened)
          {
            fclose (fi2);
            fi2_opened= 0;
          }
          break;

#ifdef ALLOW_MD5
        case 'm': case 'M':
          use_md5= 1;
          break;
        case 'n': case 'N':
          use_md5= 0;
          break;
#endif
        case 'x': case 'X':
          xformat= (int) get_parameter_value (&argv[i][2]);
          break;
        case 's': case 'S':
          {
            char *str;
            str= MD5check_string ((unsigned char *) &argv[i][2]);
            printf ("%s %s\n", str, &argv[i][2]);
          }
          break;

        case 'a': case 'A':
        case 'b': case 'B':
        case 'c': case 'C':
        case 'e': case 'E':
        case 'r': case 'R':
        case 't': case 'T':
          for (j= 1; argv [i][j]; j++)
            switch (argv [i][j])
            {
              case 'a': case 'A': disp_nf= 0;                   break;
              case 'b': case 'B': disp_ok= 0;                   break;
              case 'c': case 'C': Comparemode= 1;               break;
              case 'e': case 'E': echo_mode= 1;                 break;
              case 't': case 'T': print_summary= 0;             break;
#ifdef SUBDIRS
              case 'r': case 'R':
                wcrd_set_subdirs (1);
                break;
#endif
              case 'v': case 'V':
                verbose_mode++;
                break;
              default:
                fprintf (stderr, "unknown option %c\n", argv[i][j]);
                goto HLP;
            }
          break;

        case 'h': case 'H':
HLP:
#include <gg/help.inc>
      }
    }
    else
    {
      cmps= Comparemode;
      Comparemode= 0;

#ifdef SUBDIRS
      if (wcrd_wildcards_or_rec (argv [i]) == -1) goto STOP;
#else
      wcrd_wildcards (argv [i]);
#endif

      Comparemode= cmps;
    }
  }

STOP:
  actual_datim (line, DATIMfmt_date_time);
  time_end= strdup (line);

  if (print_summary)
  {
    if (Comparemode)
       fprintf (stderr,
         "\n%5ld Files checked: %5ld good, %5ld bad, + %5ld not found\n",
         Ncmp, Ngood, Nbad, Nfnd);

    fprintf (stderr, "start time: %s\n", time_start);
    fprintf (stderr, "end   time: %s\n", time_end);
    fprintf (stderr, "total: %ld files; %ld bytes\n",
      T_num_files, T_siz_files);
  }

  return 0;
}

/* ------------------------------------------------------------------------ */
int main_fnc (char *fnm)
{
  long cnt1, cnt2, cnt3;
  int  cnt4;

  return check_file (fnm, cnt1, cnt2, cnt3, cnt4, "");
}

/* ------------------------------------------------------------------------ */
char *CMPl (long l1, long l2)
{
  if (l1 == l2)
         return "good";
  else {
         badflag= 1;
         return "bad";
       }
}

/* ------------------------------------------------------------------------ */
char *CMPi (int l1, int l2)
{
  if (l1 == l2)
         return "good";
  else {
         badflag= 1;
         return "bad";
       }
}

/* ------------------------------------------------------------------------ */
int check_file (
char *fnm,
long c1,
long c2,
long c3,
int c4,
char *xdigest_string)
{
  FILE *fi;
  int ch, msb_lsb;
  long cnt1;      /* File Laenge */
  long cnt2;      /* Byte Count  */
  long cnt3;      /* Word Count  */
  int  cnt4;      /* Word XOR    */
  char *cp1, *cp2, *cp3, *cp4;
#ifdef ALLOW_MD5
  MD5_CTX md5_context;
  unsigned char digest [18];
  char digest_string [40];
  unsigned int len;
#endif
  struct stat f_st;
  long cur_time;
  long mod_time;

#ifdef MSDOS
  to_lower (fnm);
  xchange (fnm, '\\', '/');
#endif

  T_num_files++;

  if (stat (fnm, &f_st) != 0
      /* || (fi= fopen (fnm, "rb")) == (FILE *) 0 */
      || (fi= fopen (fnm, "r")) == (FILE *) 0
      )
  {
    Nfnd++;

    switch (xformat)
    {
      case CHECKSUM_FORMAT_NEW:
      case CHECKSUM_FORMAT_EXTENSIVE:
        if (Comparemode)
        {
          printf ("%-40s %s\n", fnm, NOT_OPENED);
          if (echo_mode)
            fprintf (stderr, "%-40s %s\n", fnm, NOT_OPENED);
        }
        else
        {
          printf ("%-32s **** %8ld %s\n", NOT_OPENED, -1L, fnm);
          if (echo_mode)
            fprintf (stderr, "%-32s **** %8ld %s\n", NOT_OPENED, -1L, fnm);
        }
        break;

      default:
        if (disp_nf) 
         printf (         "%-40s     ########## not opened #########\n", fnm);
        if (echo_mode && disp_nf)
         fprintf (stderr, "%-40s     ########## not opened #########\n", fnm);
        break;
    }

    return 0;
  }

  cnt1= cnt2= cnt3= (long) 0;
  cnt4= 0;
  msb_lsb= 256;
#ifdef ALLOW_MD5
  if (use_md5) MD5Init (&md5_context);
#endif

  cur_time= time ((time_t *) 0);
  mod_time= f_st.st_mtime;

  for (;;)
  {
#ifdef ALLOW_MD5
    if (use_md5)
    {
      len= fread (MD5_buffer, 1, MD5_bufsiz, fi);
      if (len <= 0) break;
      MD5Update (&md5_context, MD5_buffer, len);
      cnt1 += (long) len;
printf ("cnt1=%ld len=%ld\n", cnt1, len);
    }
    else
#endif
    {
      ch= fgetc (fi);
      if (feof (fi)) break;
      cnt1++;
      cnt2 += (long) ch;
      cnt3 += (long) (msb_lsb*ch);
      if (msb_lsb == 256) cnt4 ^= ((ch << 8) & 0xFF00);
      else                cnt4 ^= ( ch       & 0x00FF);
      msb_lsb= (msb_lsb == 256) ? 1 : 256;
    }
  }
  fclose (fi);

#ifdef ALLOW_MD5
  if (use_md5)
  {
    MD5Final (digest, &md5_context);
    bin_to_hex ((char *) digest, digest_string, 16);
#ifdef OLD /* 1995-04-01 10:15 */
printf ("%s <<<< bin_to_hex\n", digest_string);
    MD5Digest2String ((char *) digest, digest_string);
printf ("%s <<<< md5dig2str\n", digest_string);
#endif /* OLD */
  }
#endif

  if (Comparemode)
  {
    badflag= 0;
#ifdef ALLOW_MD5
    if (use_md5)
    {
      badflag= strcmp (digest_string, xdigest_string);
      if (disp_ok || badflag)
      {
        printf ("%-40s MD5 finger print %s\n",
                fnm, (badflag) ? "bad" : "ok");
        if (echo_mode)
          fprintf (stderr, "%-40s MD5 finger print %s\n",
                   fnm, (badflag) ? "bad" : "ok");
      }
    }
    else
#endif
    {
      cp1= CMPl (cnt1, c1);
      cp2= CMPl (cnt2, c2);
      cp3= CMPl (cnt3, c3);
      cp4= CMPi (cnt4, c4);
      if (disp_ok || badflag)
      {
        printf (          "%-40s %8s %8s %8s %8s\n", fnm, cp1, cp2, cp3, cp4);
        if (echo_mode)
         fprintf (stderr, "%-40s %8s %8s %8s %8s\n", fnm, cp1, cp2, cp3, cp4);
      }
    }
    if (badflag) Nbad++; else Ngood++;
    Ncmp++;
  }
  else
  {
#ifdef ALLOW_MD5
    if (use_md5)
    {
      switch (xformat)
      {
        case CHECKSUM_FORMAT_NEW:
          printf ("%s file %8ld %s\n", digest_string, cnt1, fnm);
          if (echo_mode)
            fprintf (stderr, "%s file %8ld %s\n", digest_string, cnt1, fnm);
          break;

        case CHECKSUM_FORMAT_EXTENSIVE:
          printf ("%s %8ld %8ld file %8ld %s\n",
                  digest_string, cur_time, mod_time, cnt1, fnm);
          if (echo_mode)
            fprintf (stderr, "%s %8ld %8ld file %8ld %s\n",
                     digest_string, cur_time, mod_time, cnt1, fnm);
          break;

        default:
          printf ("%-40s %s\n", fnm, digest_string);
          if (echo_mode) fprintf (stderr, "%-40s %s\n", fnm, digest_string);
          break;
      }
    }
    else
#endif
    {
      printf ("%-40s %8lx %8lx %8lx %8x\n",
              fnm, cnt1, cnt2, cnt3, cnt4);
      if (echo_mode)
        fprintf (stderr, "%-40s %8lx %8lx %8lx %8x\n",
                 fnm, cnt1, cnt2, cnt3, cnt4);
    }
  }

  T_siz_files += cnt1;

  return 0;
}
