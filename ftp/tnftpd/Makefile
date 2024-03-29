PORTNAME=	tnftpd
DISTVERSION=	20231001
PORTREVISION=	1
CATEGORIES=	ftp
MASTER_SITES=	https://cdn.netbsd.org/pub/NetBSD/misc/tnftp/ \
		https://ftp.uni-erlangen.de/netbsd/misc/tnftp/ \
		https://ftp.jaist.ac.jp/pub/NetBSD/misc/tnftp/ \
		https://ftp.netbsd.org/pub/NetBSD/misc/tnftp/

MAINTAINER=	cejkar@fit.vutbr.cz
COMMENT=	Enhanced FTP server from NetBSD
WWW=		https://ftp.netbsd.org/pub/NetBSD/misc/tnftp/

LICENSE=	BSD2CLAUSE BSD3CLAUSE
LICENSE_COMB=	multi
LICENSE_FILE=	${WRKSRC}/COPYING

USES=		autoreconf libtool
GNU_CONFIGURE=	yes

CFLAGS+=	-DSUPPORT_UTMPX -fcommon

SUB_FILES=	pkg-message

PLIST_FILES=	libexec/tnftpd \
		share/man/man5/ftpd.conf.5.gz \
		share/man/man5/ftpusers.5.gz \
		share/man/man8/tnftpd.8.gz
PORTDOCS=	COPYING ChangeLog NEWS README THANKS
PORTEXAMPLES=	ftpd.conf ftpusers

OPTIONS_DEFINE=	DOCS EXAMPLES

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/src/tnftpd ${STAGEDIR}${PREFIX}/libexec
	${INSTALL_MAN} ${WRKSRC}/src/ftpd.conf.man ${STAGEDIR}${PREFIX}/share/man/man5/ftpd.conf.5
	${INSTALL_MAN} ${WRKSRC}/src/ftpusers.man ${STAGEDIR}${PREFIX}/share/man/man5/ftpusers.5
	${INSTALL_MAN} ${WRKSRC}/src/tnftpd.man ${STAGEDIR}${PREFIX}/share/man/man8/tnftpd.8

do-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
.for f in ${PORTDOCS}
	${INSTALL_DATA} ${WRKSRC}/${f} ${STAGEDIR}${DOCSDIR}
.endfor

do-install-EXAMPLES-on:
	@${MKDIR} ${STAGEDIR}${EXAMPLESDIR}
.for f in ${PORTEXAMPLES}
	${INSTALL_DATA} ${WRKSRC}/examples/${f} ${STAGEDIR}${EXAMPLESDIR}
.endfor

.include <bsd.port.mk>
