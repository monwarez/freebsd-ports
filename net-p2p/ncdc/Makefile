PORTNAME=	ncdc
PORTVERSION=	1.22.1
PORTREVISION=	4
CATEGORIES=	net-p2p
MASTER_SITES=	http://dev.yorhel.nl/download/

MAINTAINER=	earl1k@mail.ru
COMMENT=	Lightweight direct connect client with ncurses interface
WWW=		https://dev.yorhel.nl/ncdc

LICENSE=	MIT

LIB_DEPENDS=	libsqlite3.so:databases/sqlite3 \
		libpcre.so:devel/pcre \
		libgmp.so:math/gmp \
		libgnutls.so:security/gnutls \
		libgcrypt.so:security/libgcrypt \
		libnettle.so:security/nettle \
		libp11-kit.so:security/p11-kit

USES=		compiler:c11 gmake gnome pkgconfig ncurses
USE_GNOME=	glib20
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
PLIST_FILES=	bin/ncdc \
		share/man/man1/ncdc.1.gz

CONFIGURE_ENV=	NCURSES_CFLAGS="-L${NCURSESLIB}" NCURSES_LIBS="-lncursesw"
CFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

OPTIONS_DEFINE=		GEOIP
OPTIONS_DEFAULT=	GEOIP

GEOIP_CONFIGURE_WITH=	geoip
GEOIP_CONFIGURE_ENV=	GEOIP_LIBS="-L${LOCALBASE}/lib -lmaxminddb"
GEOIP_LIB_DEPENDS=	libmaxminddb.so:net/libmaxminddb

.include <bsd.port.mk>
