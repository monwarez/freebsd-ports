PORTNAME=	libmikmod
PORTVERSION=	3.3.11.1
PORTREVISION=	2
CATEGORIES=	audio
MASTER_SITES=	SF/mikmod/libmikmod/${PORTVERSION}/

MAINTAINER=	neuroworker@gmail.com
COMMENT=	MikMod Sound Library
WWW=		https://mikmod.sourceforge.net/

LICENSE=	LGPL21

USES=		cpe gmake libtool pathfix pkgconfig
CPE_VENDOR=	raphael_assenat
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
CONFIGURE_ARGS=	--disable-esd
USE_LDCONFIG=	yes

INFO=		mikmod

post-install:
	@${STRIP_CMD} ${STAGEDIR}${PREFIX}/lib/${PORTNAME}.so.3.3.0

.include <bsd.port.mk>
