/*
 *  include FILE <gg/dpp.h>
 *
 *  Data Portability Package
 *
 *  written:       1992-00-00
 *  latest update: 1999-11-28 12:13:14
 *
 */

#ifndef __GG_dpp__
#define __GG_dpp__

#include <gg/floskel.h>

#ifdef MSDOS
#define __IS_BIG_ENDIAN__
#endif

#ifdef __linux
#ifndef __IS_BIG_ENDIAN__
#define __IS_BIG_ENDIAN__
#endif
#endif

#ifdef __FreeBSD__
#ifndef __IS_BIG_ENDIAN__
#define __IS_BIG_ENDIAN__
#endif
#endif

#ifdef _AIX
#define __IS_LITTLE_ENDIAN__
#endif
#ifdef __hpux
#define __IS_LITTLE_ENDIAN__
#endif

/* portable encoding of integer values up to 4 bytes: --------------------- */
#ifdef _FILE_DEFINED
/* dpp_0001.c */ long cdecl dpp_fread_long (FILE *fi, int bytes);
/* dpp_0002.c */ int cdecl dpp_fwrite_long (FILE *fo, long val, int bytes);
#endif

/* dpp_0003.c */ long cdecl dpp_get_long (char *buf, int bytes);
/*** long cdecl dpp_put_long (char *buf, long val, int bytes); ***/
/* dpp_0004.c */ int cdecl ARGDeval (char *s, int o, int c, int b, int d);
/* dpp_0005.c */ int cdecl ARGeval (char *s, int o, int c, int b);
/* dpp_0006.c */ int cdecl ARG_long_eval (char *s, int o, int c, int b,
                       long *val);
/* dpp_0007.c */ int cdecl msdos_get_int (char *s);
/* dpp_0008.c */ long cdecl msdos_get_long (char *s);

/* param001.c */ long cdecl _get_parameter_value (char *s, int cnt);
#ifdef NO_MACRO
/* param002.c */ long cdecl get_parameter_value (char *s);
#else
#define get_parameter_value(s) _get_parameter_value((s),1024)
#endif

/* rndlong  */ long cdecl random_long (void);
/* bin2hex  */ int cdecl bin_to_hex (char *bin, char *hex, int lng);
/* bin2hexr */ int cdecl bin_to_hex_rev (char *bin, char *hex, int lng);
/* hex2bin  */ int cdecl hex_to_bin (char *hex, char *bin, int lng);
/* lng2hex  */ int cdecl long_to_hex (long bin, char *hex);
/* lng2nstr */ int cdecl long_to_num_string (long bin, char *hex);

#endif /* __GG_dpp__ */
