PORTNAME=	nsping
PORTVERSION=	0.8
PORTREVISION=	3
CATEGORIES=	dns
MASTER_SITES=	LOCAL/roam
DISTFILES=	nsping.tar.gz

MAINTAINER=	ports@FreeBSD.org
COMMENT=	DNS "ping"

PLIST_FILES=	sbin/nsping share/man/man8/nsping.8.gz

post-patch:
	${REINPLACE_CMD} -e "s/dprintf/debug_printf/g" ${WRKSRC}/nsping.*

do-install:
		${INSTALL_PROGRAM} ${WRKSRC}/nsping ${STAGEDIR}${PREFIX}/sbin
		${INSTALL_DATA} ${WRKSRC}/nsping.8 ${STAGEDIR}${PREFIX}/share/man/man8

.include <bsd.port.mk>
