PORTNAME=	liboauth
PORTVERSION=	1.0.3
PORTREVISION=	5
CATEGORIES=	net
MASTER_SITES=	SF/${PORTNAME}

MAINTAINER=	hrs@FreeBSD.org
COMMENT=	C library implementing the OAuth Core standard
WWW=		http://liboauth.sourceforge.net/

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/COPYING.MIT

LIB_DEPENDS=	libcurl.so:ftp/curl

USES=		libtool pkgconfig ssl
USE_LDCONFIG=	yes

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
MAKE_ARGS=	pkgconfigdir="${PREFIX}/libdata/pkgconfig"
INSTALL_TARGET=	install-strip

post-patch:
	@${REINPLACE_CMD} -e '/PC_REQ=/s/ libcrypto//' ${WRKSRC}/configure

.include <bsd.port.mk>
