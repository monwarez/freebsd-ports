PORTNAME=	repos-style
PORTVERSION=	2.0
PORTREVISION=	3
CATEGORIES=	www
MASTER_SITES=	ftp://ftp.renatasystems.org/pub/FreeBSD/ports/distfiles/

MAINTAINER=	avk@vl.ru
COMMENT=	Stylesheet for the Subversion index web page
WWW=		http://www.reposstyle.com/

.if ${WITH_SUBVERSION_VER:U} == LTS
RUN_DEPENDS=	subversion-lts>0:devel/subversion-lts
.else
RUN_DEPENDS=	subversion>0:devel/subversion
.endif

USES=		dos2unix php:web zip
DOS2UNIX_GLOB=	*.xsl *.php
NO_BUILD=	YES

USE_PHP=	xml

WRKSRC=		${WRKDIR}/repos-web

SUB_LIST+=	PORTNAME=${PORTNAME}
SUB_FILES+=	pkg-message

pre-install:
# Prevent installation of .orig files by deleting them.
	@${FIND} ${WRKSRC} -name '*.orig' -delete
	${MV} ${WRKSRC}/open/log/index.php ${WRKSRC}/open/log/index.dist.php

do-install:
	@${MKDIR} ${STAGEDIR}${WWWDIR}
	@(cd ${WRKSRC} && ${PAX} -r -w . ${STAGEDIR}${WWWDIR})

.include <bsd.port.mk>
