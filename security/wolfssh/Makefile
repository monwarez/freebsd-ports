PORTNAME=	wolfssh
PORTVERSION=	1.4.15
CATEGORIES=	security
MASTER_SITES=	https://www.wolfssl.com/

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Lightweight SSH Library
WWW=		https://www.wolfssl.com/products/wolfssh/ \
		https://github.com/wolfSSL/wolfssh

LICENSE=	GPLv3

LIB_DEPENDS=	libwolfssl.so:security/wolfssl

USES=		libtool localbase

CONFIGURE_ARGS=	--disable-examples \
		--enable-fwd \
		--enable-keygen \
		--enable-scp \
		--enable-sftp \
		--enable-shared=yes \
		--enable-shell \
		--enable-static=yes
GNU_CONFIGURE=	yes
INSTALL_TARGET=	install-strip
TEST_TARGET=	check
USE_LDCONFIG=	yes

.include <bsd.port.mk>
