#
# FILE Makefile
#
# C programs for the MD5 Catalog
#
# written: 2000-01-23
# $Id: Makefile,v 1.4 2015/10/15 04:49:12 gonter Exp $
#

# modules from libgg.a
OBJS= bin2hex.o dt001.o dt001b.o dt014a.o dt015a.o freadlin.o md5c.o \
      md5chk2b.o md5chkb.o md5chks.o param001.o str_0024.o
EXES= checksum t_cks02

all : $(EXES)

clean :
	rm -f $(OBJS) $(EXES) checksum.o libmd5cat.a

.c.o :
	cc -I. -c $?

# ---------------------------------------------------------------------------
CHECKSUM= checksum.o 
checksum : $(CHECKSUM) libmd5cat.a
	cc -o $@ $(CHECKSUM) libmd5cat.a

T_CKS02= t_cks02.o 
t_cks02 : $(T_CKS02) libmd5cat.a
	cc -o $@ $(T_CKS02) libmd5cat.a

libmd5cat.a : $(OBJS)
	ar ru $@ $?
