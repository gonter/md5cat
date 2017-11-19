# $Id: Makefile,v 1.6 2012/05/15 04:12:18 gonter Exp $

CKS= ../../checksum
MDC= ../../md5cat

BFF_DIR=   bff
STAGE_DIR= stage
STAGE_BIN= ${STAGE_DIR}/usr/local/bin
BINARIES=  ${STAGE_BIN}/checksum ${STAGE_BIN}/chkmd5.pl ${STAGE_BIN}/cmp2md5.pl ${STAGE_BIN}/xmd5.pl

BFF_MD5CAT= oss.md5cat.1.0.0.2.bff
PACKAGES=  ${BFF_DIR}/${BFF_MD5CAT}

all : ${STAGE_BIN} ${BFF_DIR} ${PACKAGES}

clean :
	rm -r ${STAGE_DIR} ${BFF_DIR}

# setting the stage
${STAGE_BIN} :
	-mkdir -p ${STAGE_BIN}

${BFF_DIR} :
	-mkdir -p ${BFF_DIR}

# building the binary
${CKS}/checksum :
	(cd ${CKS} && make)

${STAGE_BIN}/checksum : ${CKS}/checksum
	cp ${CKS}/checksum ${STAGE_BIN}
	chmod 755 $@

# perl scripts
${STAGE_BIN}/cmp2md5.pl : ${MDC}/cmp2md5.pl
	sed -e 's#/usr/local/bin/perl#/usr/bin/perl#' ${MDC}/cmp2md5.pl >${STAGE_BIN}/cmp2md5.pl
	chmod 755 $@

${STAGE_BIN}/chkmd5.pl : ${MDC}/chkmd5.pl
	sed -e 's#/usr/local/bin/perl#/usr/bin/perl#' ${MDC}/chkmd5.pl >${STAGE_BIN}/chkmd5.pl
	chmod 755 $@

${STAGE_BIN}/xmd5.pl : ${MDC}/xmd5.pl
	sed -e 's#/usr/local/bin/perl#/usr/bin/perl#' ${MDC}/xmd5.pl >${STAGE_BIN}/xmd5.pl
	chmod 755 $@

# packaging
${STAGE_DIR}/tmp/${BFF_MD5CAT} : ${BINARIES}
	mkinstallp -d ${STAGE_DIR} -T oss.md5cat.rte.template

${BFF_DIR}/${BFF_MD5CAT} : ${STAGE_DIR}/tmp/${BFF_MD5CAT}
	cp $? ${BFF_DIR}
