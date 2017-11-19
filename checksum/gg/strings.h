/*
 *  include FILE <gg/strings.h>
 *
 *  see also: %ds/strings
 *  - constants
 *  - prototypes
 *
 *  written:       1990 06 24
 *  latest update: 1997-10-05 12:19:12
 *  $Id: strings.h,v 1.2 2005/09/04 13:59:15 gonter Exp $
 *
 */

#ifndef __GG_strings__
#define __GG_strings__

#include <gg/floskel.h>

/* ------------------------------------------------------------------------ */
struct STRLIST
{
  struct STRLIST *strl_next;
  char *strl_string;
};

/* ------------------------------------------------------------------------ */
struct OPERATION
{
  char *OP_name;                /* name to be matched with                  */
  int   OP_minimal_length;      /* minimal match length for the string      */
  long  OP_info;                /* information to be associated, its only   */
                                /* of interest for the calling function.    */
} ;

/* constants: ------------------------------------------------------------- */
#define MATCH_error       -1    /* something is wrong                       */
#define MATCH_no           0    /* no match possible                        */
#define MATCH_no_wildcard  1    /* matched without wildcard                 */
#define MATCH_wildcard     2    /* matched with wildcard                    */

#define STR_DIGITS     "0123456789"
#define STR_HEX_DIGITS "0123456789ABCDEF"
#define STR_hex_DIGITS "0123456789abcdef"
#define STR_Hex_DIGITS "0123456789abcdefABCDEF"

/* ------------------------------------------------------------------------ */
#define strlist_new() ((struct STRLIST *) calloc (sizeof (struct STRLIST), 1))

/* prototypes of library functions: --------------------------------------- */
/* 0001 */ void cdecl strnscpy (char *d, char *s, int cnt);
/* 0002 */ int cdecl strcmp_c (char *s1, char *s2);
/* 0003 */ int cdecl strcmp_ebcdic (char *s1, char *s2);
/* 0004 */ char *cdecl to_lower (char *n);
/* 0005 */ char *cdecl to_upper (char *n);
/* 0006 */ int cdecl match (char *pattern, char *str);
/* 0007 */ int cdecl string_hash (char *str, int lng, char *hash, int hlng);
/* 0008 */ int cdecl match_lng (char *s1, char *s2, int cnt);
/* 0009 */ int cdecl strcmp_us (char *s1, char *s2);
/* 0010 */ int cdecl strcmp_weighted (char *s1, char *s2);
/* 0011 */ int cdecl strcmp_cweighted (char *s1, char *s2);
/* 0012 */ int cdecl lower_case (int n);
/* 0013 */ int cdecl upper_case (int n);
/* 0014 */ int cdecl strcmp_cre (char *s1, char *s2);
/* 0015 */ int cdecl skip_re (char *str);
/* 0016 */ int cdecl find_command (char *s, char *cmds[], int first,
                       int last);
/* 0017 */ char *cdecl str_assign (char **d, char *s);
/* 0018 */ char *cdecl rotate13 (char *s);
/* 0019 */ char *cdecl strridx (char *s, int ch);
/* 0020 */ int cdecl split_string (char *src, int spl, char *flds[],
                       int n_flds);
/* 0021 */ int cdecl match_array (char *patterns[], int pattern_count,
                       char *string);
/* 0022 */ int cdecl isolate_words (char *src, char *flds[], int n_flds);
/* 0023 */ int cdecl str2fld (char *src, char *flds[], int n_flds,
                       int delimiter, int open_quote, int close_quote,
                       int escape_char);
/* 0024 */ int cdecl isolate_tokens (char *src, char *flds[], int n_flds);
/* 0025 */ int cdecl strdiff (char *diff, char *s1, char *s2,
                       int eq_char, int diff_char, int eos_char);
/* 0026 */ int cdecl isolate2words (char *src, char *flds[], int n_flds,
                       int *table);
/* 0027 */ int cdecl put_chartable (int *table, char *chars, int value);
/* 0028 */ char *cdecl locate_word (char *s, int num);
/* 0029 */ long cdecl count_char (int code, char *s, long size);
/* 0030 */ int cdecl mk_passwd (char *s, int pass_lng);
/* 0031 */ int cdecl enter_value (char *s, int pos, int val, int digits);
#ifdef _FILE_DEFINED
/* 0032 */ void cdecl blanks (FILE *fo, int cnt);
#endif
/* 0033 */ int cdecl chr_index (char *chars, int value);
/* 0034 */ int cdecl xchange (char *s, int f, int t);
/* 0035 */ int cdecl gopher_url2components (char *url, char **host,
             long *port, int *type, char **path, int *tabs);
/* 0036 */ int cdecl str_prompt (char *txt, char *dest, int lng,
             char *tmp, char *preset);
/* 0037 */ int cdecl remove_apostrophe (char *s);
/* 0038 */ char *cdecl str_or_undef (char *s);
/* 0039 */ int cdecl mk_PassWord (char *s, int pass_lng);
/* 0040 */ struct OPERATION *cdecl get_operation (struct OPERATION ops [],
             int n_ops, char *str);
/* 0041 */ int cdecl get_cat_code (int ch);
               void cdecl set_cat_code (int ch, int code);
/* 0042 */ int cdecl get_weight_code (int ch);
               void cdecl set_weight_code (int ch, int code);
/* 0043 */ int cdecl mk_pin_code (char *s, int pass_lng);
/* 0044 */ int cdecl free_or_what (void *s);
/* 0045 */ char *cdecl strdup_extra (char *str, int extra);
/* 0046 */ int cdecl sgml_encode (char *title, char *tmp, int str_size);
#ifdef _FILE_DEFINED
/* 0047 */ char *cdecl read_string (FILE *fi, int string_length);
#endif /* _FILE_DEFINED */
/* 0048 */ char *cdecl XSTR (char *str);
/* 0049 */ char *cdecl strcpy_max (char *d, char *s, int size);
/* 0050 */ int cdecl strcmp_nn (char *s1, char *s2);
/* 0051 */ char *cdecl strdup_or_what (char *s);
/* 0052 */ int cdecl strcpy2_max (char *d, char *s, int size);
/* 0053 */ char *cdecl get_password_verified (char *p1, char *p2);

/* remains of the famous MAGIS -------------------------------------------- */
/* 0107 */ void cdecl strnins (char *str, int lng, int pos, int cnt);
/* 0108 */ void cdecl strndel (char *str, int lng, int pos, int cnt);
/* 0109 */ void cdecl delstr (char *s, int p);

/* REXX like things ------------------------------------------------------- */
/* 0201 */ int cdecl abbrev (char *unabbr, char *abbr, int lng);
/* 0202 */ char *cdecl translate (char *s, char *tbl_in, char *tbl_out);
/* 0203 */ int cdecl verify (char *string, char *reference);
/* 0204 */ int cdecl strip (char *string, char *where);

/* OS/environment specific functions -------------------------------------- */
#ifdef __CMS__
#define __strdup_undefined__
#endif
#ifdef __hpux_older_version
#define __strdup_undefined__
#endif
#ifdef __ultrix
#define __strdup_undefined__
#endif
#ifdef __AIX43__
#define __strndup_defined__
#endif

#ifdef __strdup_undefined__
/* 0301 */ char *cdecl strdup (const char *str);
#endif

#ifndef __strdnup_defined__
/* 0302 */ char *cdecl strndup (const char *str, size_t size);
#endif

#ifdef __strrev_undefined__
/* 0303 */ char *cdecl strrev (char *str);
#endif

/* 0304 */ char *cdecl strdup_max (char *str, int max_size);

#endif /* __GG_strings__ */
