PORTNAME=	bzflag
PORTVERSION=	2.4.26
PORTREVISION=	2
CATEGORIES=	games
MASTER_SITES=	https://download.bzflag.org/bzflag/source/${PORTVERSION}/

MAINTAINER=	kevinz5000@gmail.com
COMMENT?=	Multiplayer 3D tank battle game
WWW=		https://bzflag.org/

LICENSE=	LGPL21
LICENSE_FILE=	${WRKSRC}/COPYING

BUILD_DEPENDS=	objdump:devel/binutils \
		${LOCALBASE}/include/linux/input.h:devel/evdev-proto
LIB_DEPENDS=	libcurl.so:ftp/curl \
		libcares.so:dns/c-ares

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
USES=		autoreconf compiler:c++11-lib libtool localbase ncurses pkgconfig tar:bzip2
INSTALL_TARGET=	install-strip

.ifndef SERVER_ONLY
USES+=		gl sdl xorg
USE_GL=		gl glu glew
USE_SDL=	sdl2
USE_XORG=	x11 xext xxf86vm sm ice xcb xshmfence xdamage xfixes
CONFLICTS_INSTALL=	bzflag-server
.endif

CPPFLAGS+=	-I ${LOCALBASE}/include

.ifdef SERVER_ONLY
CONFIGURE_ARGS+=	--disable-client
PLIST_SUB+=		CLIENT="@comment "
.else
DESKTOP_ENTRIES=	"BZFlag" "Battle enemy tanks" \
			"${DATADIR}/bzflag-256x256.png" \
			"bzflag" "Game;ActionGame;" false
PLIST_SUB+=		CLIENT=""
USES+=			desktop-file-utils
.endif

.include <bsd.port.mk>
