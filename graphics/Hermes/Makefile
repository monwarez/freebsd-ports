PORTNAME=	Hermes
PORTVERSION=	1.3.3
PORTREVISION=	5
CATEGORIES=	graphics
MASTER_SITES=	ftp://ftp.su.se/pub/lunar/source-archive/ \
		GENTOO

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Fast pixel formats conversion library
WWW=		http://web.archive.org/web/20061005045333/http://www.clanlib.org/hermes/

LICENSE=	LGPL20
LICENSE_FILE=	${WRKSRC}/COPYING

GNU_CONFIGURE=	yes
INSTALL_TARGET=	install-strip
USES=		gmake libtool tar:bzip2
USE_LDCONFIG=	yes
LDFLAGS_i386=	-Wl,-znotext

PORTDOCS=	*

OPTIONS_DEFINE=	DOCS

post-extract:
	${TOUCH} ${WRKSRC}/src/hrconfig.h.in

post-install-DOCS-on:
	@cd ${WRKSRC} && ${COPYTREE_SHARE} "AUTHORS ChangeLog FAQ README docs" \
		${STAGEDIR}${DOCSDIR} "! -name generate"

.include <bsd.port.mk>
