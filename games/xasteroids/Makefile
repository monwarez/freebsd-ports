PORTNAME=	xasteroids
PORTVERSION=	5.0
PORTREVISION=	3
CATEGORIES=	games
MASTER_SITES=	XCONTRIB/../R5contrib
DISTNAME=	${PORTNAME}
EXTRACT_SUFX=	.sh.Z

MAINTAINER=	ports@FreeBSD.org
COMMENT=	X11 based asteroids style arcade game

EXTRACT_CMD=	( cd ${WRKSRC}; zcat | ${SH} -s )
EXTRACT_BEFORE_ARGS=	<
EXTRACT_AFTER_ARGS=	# empty

NO_WRKSUBDIR=	yes

USES=		xorg
USE_XORG=	x11 xext

CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib -lXext -lX11 -lm

PLIST_FILES=	bin/xasteroids \
		share/man/man6/xasteroids.6.gz

post-extract:
	@${LN} -sf xast.man ${WRKSRC}/xasteroids.6

do-build:
	(cd ${WRKSRC} && ${CC} ${CPPFLAGS} ${CFLAGS} -o xasteroids xast.c \
		${LDFLAGS})

do-install:
	(cd ${WRKSRC} && ${INSTALL_PROGRAM} xasteroids ${STAGEDIR}${PREFIX}/bin)
	(cd ${WRKSRC} && ${INSTALL_MAN} xasteroids.6 ${STAGEDIR}${PREFIX}/share/man/man6)

.include <bsd.port.mk>
