PORTNAME=	mplex
PORTVERSION=	1.1
PORTREVISION=	1
CATEGORIES=	multimedia
MASTER_SITES=	LOCAL/sanpei

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Multiplexes MPEG component streams into system layers

LICENSE=	GPLv2+
LICENSE_FILE=	${WRKSRC}/COPYING

CONFLICTS=	mjpegtools

PLIST_FILES=	bin/mplex share/man/man1/mplex.1.gz

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/mplex ${STAGEDIR}${PREFIX}/bin
	${INSTALL_MAN} ${FILESDIR}/mplex.1 ${STAGEDIR}${PREFIX}/share/man/man1

.include <bsd.port.mk>
