PORTNAME=	openrgb
PORTVERSION=	g20220505
CATEGORIES=	x11

MAINTAINER=	vidar@karlsen.tech
COMMENT=	Software to control a multitude of (a)RGB devices

LICENSE=	GPLv2

LIB_DEPENDS=	libmbedx509.so:security/mbedtls \
		libmbedtls.so:security/mbedtls \
		libmbedcrypto.so:security/mbedtls \
		libhidapi.so:comms/hidapi \
		libGL.so:graphics/libglvnd
BUILD_DEPENDS=	mbedtls>0:security/mbedtls \
		hidapi>0:comms/hidapi \
		qt5-qmake>0:devel/qt5-qmake \
		git>0:devel/git

USES=		gmake qt:5 qmake
USE_QT=		core gui widgets buildtools
USE_GL=		gl
USE_GITLAB=	yes
GL_ACCOUNT=	CalcProgrammer1
GL_PROJECT=	OpenRGB
GL_COMMIT=	817d5d55dfb2baf932effe9fec9c557ad1529eb9

PLIST_FILES=	bin/openrgb

do-install:
		${INSTALL_PROGRAM} ${WRKSRC}/openrgb ${STAGEDIR}${PREFIX}/bin

.include <bsd.port.mk>
