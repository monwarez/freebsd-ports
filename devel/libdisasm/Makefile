PORTNAME=	libdisasm
PORTVERSION=	0.23
PORTREVISION=	3
CATEGORIES=	devel
MASTER_SITES=	SF/bastard/${PORTNAME}/${PORTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Intel x86 instructions disassembler utility and library
WWW=		https://bastard.sourceforge.net/libdisasm.html

LICENSE=	ART10
LICENSE_FILE=	${WRKSRC}/LICENSE

USES=		libtool
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
USE_LDCONFIG=	yes

PORTDOCS=	libdisasm.txt

OPTIONS_DEFINE=	DOCS

post-install:
	@${STRIP_CMD} ${STAGEDIR}${PREFIX}/lib/libdisasm.so

post-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/doc/libdisasm.txt ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
