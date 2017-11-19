/*
 *  include FILE <gg/sbr.h>
 *
 *  Deklarationen fuer Funktionen aus LSBR.LIB bzw. SSBR.LIB
 *
 *  Module
 *    freadlin.c
 *    fscanlin.c
 *    fsopen.c
 *    ftopen.c
 *    topen.c
 *    ftransfer.c
 *
 *  written:       1989 05 02
 *  latest update: 1997-11-02 14:05:21
 *
 */

#ifndef __GG_sbr__
#define __GG_sbr__

#include <gg/floskel.h>

/* Modul FREADLIN.C ------------------------------------------------------- */
#ifdef _FILE_DEFINED
int cdecl fread_line (FILE *fi, char *p, int c);
int cdecl fread_mac_line (FILE *fi, char *p, int c);
/* include <STDIO.H>                                                        */
/* int fread_line (fi, p, c)                                                */
/* FILE *fi;                                       file to read from        */
/* char *p;                                        line buffer              */
/* int c;                                          size of line buffer      */
/* RETURNS: <0 ... error or END-OF-FILE                                     */
/*           0 ... empty line                                               */
/*          >0 ... number of characters read into line buffer               */
/* REMARKS: if c is smaller than the length of line, then the               */
/*          remaining characters are read away.                             */

/* Modul FSCANLIN.C ------------------------------------------------------- */
int cdecl fscan_line (
FILE *fi,               /* gelesenes File                                   */
char *line,             /* Pufferbereich fuer Zeile                         */
int max_line,           /* Groesse von line                                 */
int max_par,            /* maximale Zahl der geparsten Zeichen              */
                        /* <<<< nicht verwendet !!!                         */
int comment,            /* Kommentarzeichen (z.b. '#')                      */
int in_flags,           /* Input Flags: siehe FCLI_xxxxx                    */
                        /* <<<< nicht verwendet !!!                         */
char *parse_line [],    /* Zeiger auf geparste Zeilenelemente               */
int parse_pos [],       /* Position der geparsten Zeilenelemente            */
int *parse_cnt,         /* tatsaechlich gelesene Zeilenelemente             */
int *out_flags);        /* Output Flags: siehe FCLO_xxxx                    */
/* RETURN: EOF wenn fgetc EOF liefert                                       */
/*         EOF wenn keine Zeile mehr geparst werden kann                    */
/*         0   alles ok                                                     */

/* Modul FSCANLIN.C ------------------------------------------------------- */
int cdecl fpars_line (
char *line,             /* Pufferbereich fuer Zeile                         */
int comment,            /* Kommentarzeichen (z.b. '#')                      */
int in_flags,           /* Input Flags: siehe FCLI_xxxxx                    */
char *parse_line [],    /* Zeiger auf geparste Zeilenelemente               */
int parse_pos [],       /* Position der geparsten Zeilenelemente            */
int *parse_cnt,         /* tatsaechlich gelesene Zeilenelemente             */
int *out_flags);        /* Output Flags: siehe FCLO_xxxx                    */
/* RETURN: EOF wenn fgetc EOF liefert                                       */
/*         EOF wenn keine Zeile mehr geparst werden kann                    */
/*         0   alles ok                                                     */

/* Modul FSOPEN.C --------------------------------------------------------- */
FILE *cdecl fsopen (char *, char *,
                     unsigned int, int, int, int *);
/* INCLUDE: <stdio.h> <share.h> <fcntl.h>                                   */
/* FILE *fsopen (file_name, file_mode,                                      */
/*               open_mode, share_mode, retry, err)                         */
/* char *file_name,        Filename des zu oeffnenden Files                 */
/*      *file_mode;        fopen (file_name, file_mode);                    */
/* int   open_mode,        open (file_name, open_mode);                     */
/*       share_mode,       Share Codes                                      */
/*       retry,            Zahl der Versuche                                */
/*      *err;              DOS Error Code; Fehlercode                       */
/* RETURN: file_pointer (Stream); err := 0                                  */
/*         (FILE *) 0 --> File not opened; err := Fehlercode                */

/* Modul FTOPEN.C --------------------------------------------------------- */
FILE *cdecl ftopen (char *, char *,
                     int, int, int *);
/* INCLUDE: <stdio.h>                                                       */
/* FILE *ftopen (path_name, file_mode,                                      */
/*               open_mode, share_mode, err)                                */
/* char *path_name,        Pathname des zu oeffnenden Files (siehe unten)   */
/*      *file_mode;        fopen (file_name, file_mode);                    */
/* int   open_mode,        open (file_name, open_mode);                     */
/*       share_mode,       Share Codes                                      */
/*      *err;              DOS Error Code; Fehlercode                       */
/* RETURN: file_pointer (Stream); err := 0                                  */
/*         (FILE *) 0 --> File not opened; err := Fehlercode                */

/* module %sbr/flopen.c --------------------------------------------------- */
#ifdef _FILE_DEFINED
FILE *cdecl flopen (char *file_name, char *open_mode);
#endif /* _FILE_DEFINED */
int cdecl flclose (char *file_name);
int cdecl flclose_all (void);
int cdecl flprint_table (FILE *fo);

#ifdef _FILE_DEFINED
/* fopenwng.c */ FILE *cdecl fopen_wng (char *fnm, char *mode);
#endif /* _FILE_DEFINED */

/* ftrans01.c */ long cdecl ftransfer (FILE *fi, FILE *fo, long cpy_size);
/* ftrans02.c */ long cdecl f2transfer (char *fi_nm, FILE *fo,
                   long cpy_start, long cpy_size);
/* ftrans03.c */ long cdecl f3transfer (char *fi_nm, char *fo_nm,
                   long cpy_start,long cpy_size);
/* ftrans04.c */ long cdecl ftransfer_to_shar (FILE *fi, FILE *SHAR,
                   char *fnm, long cpy_size, char *st_dat_buffer);
/* ftrans05.c */ long cdecl f5transfer_to_shar (char *fi_nm, FILE *SHAR,
                   char *fnm, long cpy_start, long cpy_size, char *fi_buf,
                   int shar_bufsiz, char *st_dat_buffer);
/* ftrans06.c */ long cdecl ftransfer_line (FILE *fi, FILE *fo,
                   long cpy_size);
/* ftrans07.c */ long cdecl ftransfer_crlf (FILE *fi, FILE *fo,
                   long cpy_size);
/* ftrans08.c */ long cdecl f2transfer_crlf (char *fi_nm, FILE *fo,
                   long cpy_start, long cpy_size);
/* ftrans09.c */ long cdecl ftransfer_add_dot (FILE *fi, FILE *fo,
                   long cpy_size);
/* ftrans10.c */ long cdecl ftransfer_del_dot (FILE *fi, FILE *fo,
                   long cpy_size);

/* trasheol.c */ long cdecl trash_until_eol (FILE *fi);
/* freadit.c  */ int cdecl fread_integer_table (FILE *f,
                   long int_table [], int table_size, char *buffer,
                   int buffer_size);
/* freaditf.c */ int cdecl fread_integer_table_file (char *fnm,
                   long int_table [], int table_size, char *buffer,
                   int buffer_size);
/* freadst.c  */ int cdecl fread_string_table (FILE *f,
                   char *string_table [], int table_size, char *buffer,
                   int buffer_size);
/* freadstf.c */ int cdecl fread_string_table_file (char *fnm,
                   char *string_table [], int table_size, char *buffer,
                   int buffer_size);
#endif

long cdecl copy_file (char *src, char *dst, int flags);
/* Return: -1L ... error; >= 0L number of bytes copied */
#define COPY_FILE_verify               0x0001
#define COPY_FILE_append               0x0002
#define COPY_FILE_old_date             0x0004  /* not implemented */
#define COPY_FILE_dont_overwrite       0x0008
#define COPY_FILE_in_text              0x0010
#define COPY_FILE_out_text             0x0020

int cdecl copyfile (char *src, char *dst);

/* Modul TOPEN.C ---------------------------------------------------------- */
long cdecl topen (char *path_name, int msdos_file_attribute);
/* ANMERKUNG:                                                               */
/* path_name: String des gueltigen Pfades, nach dem 0-Character muss laut   */
/*            MSDOS Programmierhandbuch noch fuer 13 Byte Platz sein.       */

/* Modul FNMQUEUE.C ------------------------------------------------------- */
int cdecl string_queue (char *fn);
int cdecl string_push (char *fn);
char *cdecl string_pull (void);
void cdecl string_reset (void);

/* module fmt.c ----------------------------------------------------------- */
void cdecl dc_reset (void);
void cdecl dc_modus (int d);
void cdecl dc_cal (double v);
char *cdecl dc_chk (int st, int sig, int nk, int faktor_st, double *faktor,
              char **cp2, int komma, int punkt);
char *cdecl dc_fmt (double v);

/* module scan.c ---------------------------------------------------------- */
/* Einlesen eines Strings via field_edit                                    */
int cdecl scan_str (
char field [],          /* editiertes Feld                                  */
int helpnum,            /* 1. Help Seite                                    */
int flag,               /* Bit 0: --> Field loeschen vor Edit               */
                        /*     1: --> Field terminieren nach Blank udgl.    */
                        /*     2: --> Default-Descriptor verwenden          */
                        /*     3: --> Editierung Ende nach 1 Input Char     */
                        /*            == Single Character Mode              */
                        /* Bit 4: --> Feld nur anzeigen                     */
int timeout1,
int x,
int y,
int l,
int attr,               /* Koordinaten, Laenge und Attribut                 */
int *oflg);             /* Bit 0: field veraendert                          */

/* module scan.c ---------------------------------------------------------- */
/* Ein Feld im Textmodus (40|80 Zeichen) editieren                          */
int cdecl field_edit (
char field [],                  /* Editiertes Feld (intern immer String)    */
int mode,                       /* 1 -> 80 Zeichen Mode                     */
char desc [],                   /* Field-Descriptor-Record                  */
unsigned int *key,              /* letzte uninterpretierte Taste            */
unsigned int *timeout,          /* (IN/OUT) Zeit Limit fuer Edit:           */
                                /* kann fuer ein ganzes Feld oder auch nur  */
                                /* fuer ein Zeichen verstanden werden       */
unsigned int *oflg);            /* Bit 0: field veraender                   */
/* RETURN: 0 -> keine Funktionstastenzuordnung                              */
/*           -> *key == uninterpretierter Tastenwert (mapped)               */
/*         1 -> Funktionstastenzuordnung gefunden                           */
/*           -> *key == Destination Frame Nummer (Bit 15 == 0)              */
/*         2 -> Timeout abgelaufen; *key ist undefiniert                    */

/* module scan.c ---------------------------------------------------------- */
void cdecl show_field (
char field [],                  /* Feldinhalt                               */
int mode,                       /* 1 -> 80 Zeichen                          */
char desc [],                   /* Beschreibung des Feldes                  */
int flag);                      /* 1 -> inverse Darstellung                 */

/* module scan.c ---------------------------------------------------------- */
int cdecl getkey (unsigned int *akt, unsigned int *timeout);

/* module scan.c ---------------------------------------------------------- */
/* Einlesen eines Integers via field_edit                                   */
void cdecl scan_int (
int *field,             /* editiertes Feld                                  */
int helpnum,            /* 1. Help Seite                                    */
int flag,               /* Bit 0: --> Field loeschen vor Edit               */
                        /*     1: --> Field terminieren nach Blank udgl.    */
                        /*     2: --> Default-Descriptor verwenden          */
int timeout1,
int x,
int y,
int l,
int attr,               /* Koordinaten, Laenge und Attribut                 */
int *oflg);             /* Bit 0: field veraendert                          */

/* module scan.c ---------------------------------------------------------- */
/* Einlesen eines HEX-Integers via field_edit                               */
void cdecl scan_hex (
int *field,             /* editiertes Feld                                  */
int helpnum,            /* 1. Help Seite                                    */
int flag,               /* Bit 0: --> Field loeschen vor Edit               */
                        /*     1: --> Field terminieren nach Blank udgl.    */
                        /*     2: --> Default-Descriptor verwenden          */
int timeout1,
int x,
int y,
int l,
int attr,               /* Koordinaten, Laenge und Attribut                 */
int *oflg);             /* Bit 0: field veraendert                          */

/* module scan.c ---------------------------------------------------------- */
void cdecl set_coff (int x);

#endif /* __GG_sbr__ */
