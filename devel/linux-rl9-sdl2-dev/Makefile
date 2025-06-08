PORTNAME=	SDL2-devel
PORTVERSION=	2.26.0
DISTVERSIONSUFFIX=	-1.el9
CATEGORIES=	devel linux
DISTNAME=	SDL2-devel-${DISTVERSIONFULL}

MAINTAINER=	emulation@FreeBSD.org
COMMENT=	Cross-platform multi-media development API (Rocky Linux ${LINUX_DIST_VER})
WWW=		https://www.libsdl.org/

LICENSE=	ZLIB

USES=		linux:rl9
USE_LDCONFIG=	yes
USE_LINUX=	xorglibs
USE_LINUX_RPM=	yes
WANT_LINUX32=	yes

SRC_DISTFILES=

CONFLICTS=	linux-c7-${PORTNAME}
#DESCR=		${PORTSDIR}/${PKGCATEGORY}/${PORTNAME}/pkg-descr
DOCSDIR=	${PREFIX}/usr/share/doc/SDL2

OPTIONS_DEFINE=	DOCS

.include <bsd.port.mk>
