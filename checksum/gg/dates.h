/*
 *  include FILE <gg/dates.h>
 *
 *  operation with dates, days, months and years
 *
 *  written:       1992 08 30
 *  latest update: 1998-08-04 12:30:15
 *
 */

#ifndef __GG_dates__
#define __GG_dates__

#include <gg/floskel.h>

/* ------------------------------------------------------------------------ */
#define DT_int        1
#define DT_str_mon    2
#define DT_str_tz    41
#define DT_int_tz    41
#define DT_str_tzm   42
#define DT_int_tzm   42
#define DT_str_junk  99
#define DT_int_day  201
#define DT_int_year 202
#define DT_int_hour 301
#define DT_int_min  302
#define DT_int_sec  303

/* date and time formats: ------------------------------------------------- */
#define DATIMfmt_none           0       /* nothing at all...                */
#define DATIMfmt_date_time      1       /* YYYY-MM-DD hh:mm:ss              */
#define DATIMfmt_date           2       /* YYYY-MM-DD                       */
#define DATIMfmt_time           3       /* hh:mm:ss (hh: 0..23)             */
#define DATIMfmt_touch          4       /* MMDDhhmmYY                       */
#define DATIMfmt_touch_gnu      5       /* MMDDhhmmCCYY.ss                  */
#define DATIMfmt_touch_linux    5
#define DATIMfmt_touch_aix      6       /* CCYYMMDDhhmm.ss                  */
#define DATIMfmt_dos_date       7       /* MM-DD-YYYY                       */
#define DATIMfmt_this_month     8       /* YYYY-MM                          */
#define DATIMfmt_month          8       /* YYYY-MM                          */
#define DATIMfmt_elm_mbox       9       /* ddd mmm dd hh:mm:ss YYYY         */
#define DATIMfmt_time2         10       /* hh:mm:ss (hh: 00..23)            */
#define DATIMfmt_next_month    11       /* YYYY-MM                          */
#define DATIMfmt_last_month    12       /* YYYY-MM                          */
#define DATIMfmt_seconds       13
#define DATIMfmt_GMT       0x1000       /* to be used in a bit combination  */

/* CC   ... century (19..20?)                                               */
/* YY   ... year (00..99)                                                   */
/* YYYY ... year (0..9999)                                                  */
/* MM   ... month (01..12)                                                  */
/* DD   ... day                                                             */
/* hh   ... hour (00..23)                                                   */
/* mm   ... minute (00..59)                                                 */
/* ss   ... second (00..59)                                                 */
/* ddd  ... day as a string                                                 */
/* mdd  ... month as a string                                               */

/* ------------------------------------------------------------------------ */
struct DATE_str
{
  int DStr_type;                        /* DT_xxxx                          */
  int DStr_value;
  int DStr_min_str;
  char *DStr_str;
} ;

/* ------------------------------------------------------------------------ */
struct DAY_INFO_EXTRA
{
  struct DAY_INFO_EXTRA *DAYIE_next;
  char *DAYIE_country;
  char *DAYIE_description;

  int DAYIE_type;
#define DAYIET_undef    0
#define DAYIET_fixed    1               /* fixed month/day                  */
                                        /* statically allocated             */
#define DAYIET_variable 2               /* variable month/day,              */
                                        /* dynamically allocated            */

  int DAYIE_class;
#define DAYIEC_standard 0
#define DAYIEC_note     1               /* not a bank holiday or such       */
#define DAYIEC_holiday  2

  int DAYIE_month;
  int DAYIE_day;
  int DAYIE_year_first;
  int DAYIE_year_last;
} ;

/* ------------------------------------------------------------------------ */
struct DAY_INFO
{
  int DAYI_year;
  int DAYI_month;
  int DAYI_day;
  int DAYI_wday;                        /* day of the week: 0..SUN, 6..SAT  */
  struct DAY_INFO_EXTRA *DAYI_extra;
} ;

#define DAYI_sunday     0
#define DAYI_monday     1
#define DAYI_tuesday    2
#define DAYI_wednesday  3
#define DAYI_thursday   4
#define DAYI_friday     5
#define DAYI_saturday   6

/* ------------------------------------------------------------------------ */
struct YEAR_INFO
{
  int YI_year;
  int YI_is_leap_year;
  int YI_days_of_month [12];
  int YI_month_beg [12];
#define YI_DI_cnt 366
  struct DAY_INFO YI_day_info [YI_DI_cnt];
#define YI_AV_COLUMNS 21        /* 3 monnths with 7 days per line           */
#define YI_AV_LINES 24          /* 4 such blocks spanning up to 6 weeks     */
  struct DAY_INFO *YI_av [YI_AV_LINES][YI_AV_COLUMNS];  /* alternative view */

  struct DAY_INFO *YI_easter;
} ;

/* ------------------------------------------------------------------------ */
/* dt001  */ int cdecl actual_datim (char *datum, int format);
/* dt001b */ int cdecl _actual_datim (char *datum, int format, long tt);
/* dt002  */ int cdecl actual_datim_int (int *jahr, int *monat, int *tag,
               int *stunde, int *minute, int *sekunde);
/* dt002b */ int cdecl get_current_year (void);
#ifdef _FILE_DEFINED
/* dt003  */ int cdecl print_date_time (FILE *fo);
#endif
/* dt004  */ void cdecl prepare_date (char *datum, char *tz, int max_lng);

/* dt006  */ int cdecl days_of_month (int year, int month);
/* dt007  */ int cdecl is_leap_year (int year);
#ifdef _TM_DEFINED
/* dt008  */ int cdecl modify_date (struct tm *d, int delta);
#endif

/* dt010  */ long cdecl day_number (int jahr, int mon, int tag);
/* dt011  */ int cdecl weekday (int jahr, int mon, int tag);
/* dt014a */ extern char *DAY_OF_WEEK3 [];
/* dt014b */ extern char *DAY_OF_WEEK2 [];
/* dt015a */ extern char *MONTH_NAME [];

/* dt021  */ int cdecl chk_datum (char *sb, char *se, int sl, int *d,
               int *e, int *beg_jahr);
/* dt022  */ int cdecl get_datum (char *s, int zone_begin, int zone_end,
               int *jahr, int *mon, int *tag);
/* dt023  */ int cdecl get_jahr_mon_tag (char *s, int *ii, int *vv,
               char *m, int ms, int *msr);
/* dt024  */ int cdecl delta_datum (int bjahr, int bmon, int btag, long *v1,
               int *t1, int ejahr, int emon, int etag, long *v2, int *t2,
               int *d, int *e);
/* dt025  */ int cdecl valid_datum (int jahr, int mon, int tag, long *v,
               int *t, int *xsj, int *xt);

/* dt031  */ int cdecl analyze_date (char *s, int *jahr, int *mon, int *tag,
               int *std, int *minutes, int *sek, int *x_timezone);
/* dt032  */ int cdecl analyze_dat_str (char *name, int dat_val [],
               int dat_typ [], int *dat_stack, int STACK_SIZE);
/* dt033  */ int cdecl check_for_timezone (int *v, int *s, char *str);
/* dt034  */ int cdecl get_easter (int year, int *month, int *day);
/* dt035  */ int cdecl get_day_index (struct YEAR_INFO *yi, int mon, int day);
/* dt036  */ struct DAY_INFO_EXTRA *cdecl create_day_info_extra (
               char *country, int d_class, char *description);
/* dt037  */ struct DAY_INFO *cdecl fixup_day (struct YEAR_INFO *yi,
               int idx, struct DAY_INFO_EXTRA *dep);
/* dt038  */ int get_wday_index (struct YEAR_INFO *yi, int mon, int wday,
               int num);

/* dt041  */ int cdecl setup_year_info (struct YEAR_INFO *yi, int year);
/* dt042  */ int cdecl cal_reset_year_info (struct YEAR_INFO *yi);

#ifdef _FILE_DEFINED
/* cal01  */ int print_yi_year (FILE *fo, struct YEAR_INFO *yi, int year,
               char *frame_name, int year_prev, int year_next, char *pfx);
/* cal02  */ int print_yi_months (FILE *fo, struct YEAR_INFO *yi,
               int year_prev, int year_next, char *pfx);
/* cal03  */ int print_yi_days (FILE *fo, struct YEAR_INFO *yi, char *pfx);
#endif /* _FILE_DEFINED */

/* cal04  */ extern struct DAY_INFO_EXTRA AT_holiday [];
/* cal05  */ int cdecl fixup_extra_information (struct YEAR_INFO *yi,
               struct DAY_INFO_EXTRA de []);
/* cal06  */ int cdecl get_day_string (char *day_frame, struct DAY_INFO *di);

#ifdef _FILE_DEFINED
/* cal07  */ int cdecl print_calendar (FILE *fo, int year_from,
               int year_to, int print_what, char *pfx);
#endif /* _FILE_DEFINED */
#define CAL_PRINT_root_frame            0x0001
#define CAL_PRINT_year                  0x0002
#define CAL_PRINT_months                0x0004
#define CAL_PRINT_days                  0x0008
#define CAL_PRINT_alternative_view      0x0010

/* cal08 */ int cdecl cal_reset_alternative_view (struct YEAR_INFO *yi);
/* cal09 */ int cdecl cal_setup_alternative_view (struct YEAR_INFO *yi);
#ifdef _FILE_DEFINED
/* cal10 */ int cdecl cal_print_alternative_view (FILE *fo,
              struct YEAR_INFO *yi, char *pfx);
#endif /* _FILE_DEFINED */

/* ------------------------------------------------------------------------ */
#ifdef _FILE_DEFINED
/* planner0.c */ int cdecl generate_week (FILE *fo, int week_from,
                   int week_to);
#ifdef _TM_DEFINED
                 int cdecl setup_week (struct tm *day [], int days, int wko);

/* planner1.c */ int cdecl print_week (FILE *fo, struct tm *day[], int days,
                   char *period, int language);
#endif /* _TM_DEFINED */

/* plannerl.c */ int cdecl latex_header (FILE *fo);
                 int cdecl latex_trailer (FILE *fo);
#endif /* _FILE_DEFINED */

#endif /* __GG_dates__ */
