PORTNAME=	mxml
DISTVERSION=	3.1
PORTREVISION=	1
CATEGORIES=	textproc
MASTER_SITES=	https://github.com/michaelrsweet/mxml/releases/download/v${DISTVERSION}/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Small XML parsing library
WWW=		http://www.minixml.org/

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE

USES=		desthack
USE_LDCONFIG=	yes

GNU_CONFIGURE=	yes
MAKE_ARGS=	INSTALL_DATA="${INSTALL_DATA}" \
		INSTALL_LIB="${INSTALL_LIB}" \
		INSTALL_MAN="${INSTALL_MAN}"

OPTIONS_DEFINE=	DOCS

post-patch:
	@${REINPLACE_CMD} -e '/^\.SILENT/d' -e '/(INSTALL_LIB) libmxml.a/ s/_LIB/_DATA/' \
		${WRKSRC}/Makefile.in

.include <bsd.port.mk>
