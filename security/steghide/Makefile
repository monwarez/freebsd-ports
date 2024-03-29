PORTNAME=	steghide
PORTVERSION=	0.5.1
PORTREVISION=	9
CATEGORIES=	security
MASTER_SITES=	SF

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Steganography tool to hide data in binary files
WWW=		https://steghide.sourceforge.net/

LICENSE=	GPLv2
LICENSE_FILE=	${WRKSRC}/COPYING

LIB_DEPENDS=	libmcrypt.so:security/libmcrypt \
		libmhash.so:security/mhash

USES=		compiler cpe gmake jpeg libtool:build localbase perl5
CPE_VENDOR=	${PORTNAME}_project
USE_CXXSTD=	c++14

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share

MAKE_ARGS=	LIBTOOL="${LOCALBASE}/bin/libtool"

CPPFLAGS+=	-fpermissive
CXXFLAGS+=	${CXXFLAGS_${CHOSEN_COMPILER_TYPE}}
CXXFLAGS_clang=	-Wno-static-float-init

LIBS+=		-lmcrypt

OPTIONS_DEFINE=	DOCS NLS
OPTIONS_SUB=	yes

NLS_USES=	gettext
NLS_CONFIGURE_ENABLE=	nls
NLS_LIBS=	-lintl

post-patch:
	@${REINPLACE_CMD} '/CXXFLAGS=/s|-O2|${CXXFLAGS}|' ${WRKSRC}/configure

.include <bsd.port.mk>
