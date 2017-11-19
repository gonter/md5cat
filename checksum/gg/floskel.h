/*
 *  include FILE <gg/floskel.h>
 *
 *  general clauses for header files
 *
 *  written:       1994-12-03
 *  latest update: 1996-12-11 12:46:31
 *
 */

#ifndef __GG_floskel__
#define __GG_floskel__

#ifndef MSDOS
#ifndef cdecl
#define cdecl
#endif
#endif

#ifdef _STDIO_H
#ifndef _FILE_DEFINED
#define _FILE_DEFINED
#endif
#endif

#ifdef stdout
#ifndef _FILE_DEFINED
#define _FILE_DEFINED
#endif
#endif

#ifdef _TIME_H_
#ifndef _TM_DEFINED
#define _TM_DEFINED
#endif
#endif

/* used for checking the correct assignment of function parameters */
#define PAR(name,value) (value)

#endif /* __GG_floskel__ */
