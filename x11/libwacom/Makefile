PORTNAME=	libwacom
DISTVERSION=	1.5
PORTREVISION=	1
CATEGORIES=	x11
MASTER_SITES=	https://github.com/linuxwacom/${PORTNAME}/releases/download/${PORTNAME}-${DISTVERSION}/

MAINTAINER=	x11@FreeBSD.org
COMMENT=	Adds tablet support to libinput
WWW=		https://github.com/linuxwacom/libwacom/wiki

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/COPYING

BUILD_DEPENDS=	${LOCALBASE}/include/linux/input.h:devel/evdev-proto
LIB_DEPENDS=	libgudev-1.0.so:devel/libgudev
TEST_DEPENDS=	bash:shells/bash

USES=		compiler:c++11-lang gnome meson pkgconfig tar:bz2
USE_GNOME=	glib20
USE_LDCONFIG=	yes
MESON_ARGS=	-Ddocumentation=disabled

.include <bsd.port.mk>
