PORTNAME=	bbkeys
PORTVERSION=	0.9.1
PORTREVISION=	2
CATEGORIES=	x11-wm
MASTER_SITES=	SF

MAINTAINER=	A.J.Caines@halplant.com
COMMENT=	Keygrabber for the Blackbox window manager
WWW=		https://bbkeys.sourceforge.net/

LICENSE=	MIT

BUILD_DEPENDS=	blackbox:x11-wm/blackbox

USES=		perl5 pkgconfig xorg
USE_PERL5=	build
USE_XORG=	x11 xext
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
CPPFLAGS+=	`pkg-config --cflags xft`
LDFLAGS+=	`pkg-config --libs xft`

PORTDOCS=	AUTHORS BUGS ChangeLog NEWS README TODO

PLIST_FILES=	bin/bbkeys %%DATADIR%%/bbkeysrc %%DATADIR%%/defaultStyle \
		share/man/man1/bbkeys.1.gz share/man/man5/bbkeysrc.5.gz

OPTIONS_DEFINE=	DOCS

post-patch:
	@${REINPLACE_CMD} -e 's#: install-docDATA#:#' ${WRKSRC}/Makefile.in

post-install:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${PORTDOCS:S#^#${WRKSRC}/#} ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
