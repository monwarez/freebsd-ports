PORTNAME=	tilp2
PORTVERSION=	1.18
PORTREVISION=	3
CATEGORIES=	comms
MASTER_SITES=	SF/tilp/tilp2-linux/${DISTNAME}

MAINTAINER=	danfe@FreeBSD.org
COMMENT=	TI Linking Program
WWW=		http://tilp.info/

LICENSE=	GPLv2

LIB_DEPENDS=	libticables2.so:comms/libticables2	\
		libtifiles2.so:devel/libtifiles2	\
		libticalcs2.so:comms/libticalcs2	\
		libticonv.so:converters/libticonv

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=	${PREFIX}/share
INSTALL_TARGET=	install-strip
USES=		autoreconf compiler:c11 desktop-file-utils gettext gmake gnome \
		groff libtool localbase pkgconfig shared-mime-info tar:bzip2
USE_GNOME=	gtk20 intltool

OPTIONS_DEFINE=	NLS
OPTIONS_SUB=	yes

NLS_CONFIGURE_ENABLE=	nls

post-patch:
	@${REINPLACE_CMD} -e 's,/usr,${LOCALBASE},' ${WRKSRC}/src/bookmark.c

.include <bsd.port.mk>
