/*
 *  include FILE <gg/filename.h>
 *
 *  filename operations
 *
 *  written:       1991 11 02
 *  latest update: 1998-02-28  9:33:26
 *
 */

#ifndef __GG_filename__
#define __GG_filename__

#include <gg/floskel.h>

#ifdef MSDOS
#define DEF_FNM_LENGTH 258
#else
#define DEF_FNM_LENGTH 1026
#endif
#define DEF_FNM_EXTRA 24

#ifndef MAX_FNM_LENGTH
#ifdef MSDOS
#define MAX_FNM_LENGTH 258
#else
#define MAX_FNM_LENGTH 1026
#endif
#endif

#define DEFAULT_MODULE_TABLE "module.tfb"
#define DEFAULT_MODULE_DIR   "%sbr"

/* ------------------------------------------------------------------------ */
typedef char *FVF_get_directory_listing (char *path, void *cd);

/* ------------------------------------------------------------------------ */
struct VIRTUAL_FILENAME
{
  struct VIRTUAL_FILENAME *VFN_next;
  char *VFN_virtual;
  char *VFN_real;
} ;

/* prototypes ------------------------------------------------------------- */
/* handling of logic, real and virtual paths */
/* logicfnm.c */ char *cdecl translate_logic_filename (char *fnm);
                 char *cdecl _translate_logic_filename (char *fnm,
                   char **vfnv);
                 struct VIRTUAL_FILENAME *cdecl read_logic_filename_table (
                   char *l1, int read_all);
                 struct VIRTUAL_FILENAME *cdecl _read_logic_filename_table (
                   char *lp_file, char *l1, int read_all);
                 struct VIRTUAL_FILENAME *cdecl _define_logic_filename (
                   char *fn_virtual, char *fn_real);
                 int cdecl find_logic_filename (char *p, char *vp_buffer,
                   int vp_lng);
                 struct VIRTUAL_FILENAME *get_virtual_filename_table (void);
/* fnmdef.c   */ int cdecl filename_definition (char *spec,
                    char **real, char **virtual_fnm, char *assumed_path);
/* fnmreal.c  */ char *cdecl real_filename (char *spec, char *assumed_path);
/* fnmrenm.c  */ int cdecl filename_rename (char *from, char *to, int mode);
/* fnmdpath.c */ char *cdecl duplicate_assumed_path (char *assumed_path);
/* getcdir0.c */ char *cdecl _get_current_directory (char *buffer);
/* getcdir1.c */ char *cdecl get_current_directory (char *buffer);
/* curdisk.c  */ int get_current_disk (void);
                 int select_disk (int disk_number);
/* dcd01.c    */ int cdecl dcd (char *dir_specified);

/* fnmdestn.c */ char *cdecl get_destination_name (char *dest_dir,
                   char *from, char *result, int mk_path, int rotfnm);
/* makepath.c */ int cdecl make_path (char *path, int flags);
/* fmtdta01.c */ int cdecl fmt_dta_date (char *puffer, int fn_date);
/* fmtdta02.c */ int cdecl fmt_dta_time (char *puffer, int fn_time);
/* fnmcpy.c   */ int cdecl fnmcpy (char *dst, char *src, char *ext);
/* fnmcpy2.c  */ char *fnmcpy2 (char *fnm_old, char *ext);
/* fnmcpy3.c  */ char *fnmcpy3 (char **dst, char *fnm_old, char *ext);
/* fnmcpy4.c  */ char *fnmcpy4 (char *fnm1, char *fnm2, char *ext);
/* fnmsep1.c  */ int cdecl isolate_path (char *s);
/* fnmsep2.c  */ char *cdecl remove_extension (char *s);
/* fnmsep3.c  */ char *cdecl get_filename_from_path (char *s);
/* fnmsep4.c  */ char *cdecl get_relative_path (char *p1, char *p2);
/* fndvfile.c */ int find_virtual_file (char *fnm, char *current_path,
                   char **real_fnm, char **virtual_fnm);
                 int fvf_set_dirinf (char *s);
                 int fvf_set_dirinf_callback (
                   FVF_get_directory_listing *proc, void *client_data);
/* gennednm.c */ char *cdecl generate_ned_name (char *fnm, char *ext,
                   int check_subdir);
/* getpwd_l.s */ void cdecl getpwd (char *buffer, int drive);
/* extcmp.c   */ int cdecl extcmp (char *s1, char *s2);
/* setetcfn.c */ char *cdecl set_etc_filename (char *dst, char *fnm);
/* settmpfn.c */ char *cdecl set_tmp_filename (char *dst, char *fnm);
/* envfilsr.c */ char *cdecl env_file_search (char *fn, char *env);
/* envbinsr.c */ char *cdecl env_bin_search (char *fn, char *env,
                   char *ext [], int n_ext);
/* shift01.c  */ int cdecl vshift (char *fnm, long version, char *fmt,
                   int doit, int verbose);
/* tellext.c  */ char *cdecl tell_extension (char *s);
/* fnm_d2u.c  */ int cdecl filename_dos2unix (char *d, char *s);
/* wildcrd2.c */ int cdecl wcrd_wild_check (char *nm);
/* modtbl.c   */ int cdecl read_module_table (char *fnm);
                 char *cdecl get_module_directory (char *fnm);
/* cdromfnm.c */ int cdecl cdrom_file_name (char *fnm);
/* fnmovwr.c  */ char *alternative_filename (char *fnm, int extra);

/* module rotfile.c ------------------------------------------------------- */
#ifdef _FILE_DEFINED
int cdecl rotate_file (char *fn, FILE *fo, int verbose_mode, int decode_it,
  int rotate_names);
#endif

/* module wildcard.c ------------------------------------------------------ */
#ifndef __CMS__         /* what other operating system would care for this? */
int cdecl wcrd_get_date (void);
int cdecl wcrd_get_time (void);
long cdecl wcrd_get_size (void);
int cdecl wcrd_wildcards_or_rec (char *fn);
int cdecl wcrd_wildcards (char *fn);
int cdecl wcrd_set_match_mode (int mode);
int cdecl wcrd_get_match_mode (void);
int cdecl wcrd_set_subdirs (int mode);
#define wcrd_set_recursive wcrd_set_subdirs
int cdecl wcrd_get_subdirs (void);
int cdecl wcrd_get_current_level (void);
int cdecl wcrd_leave (int levels);

#ifdef __COMMENT__
int cdecl main_fnc (char *fn);  /* this function will be called from the    */
                                /* the wildcard functions and performs the  */
                                /* actual processing task with the file     */
#endif /* __COMMENT__ */
#endif /* !__CMS__ */


#endif /* __GG_filename__ */
