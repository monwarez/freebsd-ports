PORTNAME=	html2xhtml
PORTVERSION=	1.3
PORTREVISION=	1
CATEGORIES=	textproc
MASTER_SITES=	http://www.it.uc3m.es/jaf/html2xhtml/downloads/

MAINTAINER=	wilfried.meindl@gmail.com
COMMENT=	Convert HTML to XHTML
WWW=		https://www.it.uc3m.es/jaf/html2xhtml

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
USES=		iconv

CONFLICTS_INSTALL=	p5-HTML-HTML5-Parser p5-XML-Driver-HTML # bin/html2xhtml

PLIST_FILES=	bin/dtdquery \
		bin/html2xhtml \
		share/man/man1/html2xhtml.1.gz

.include <bsd.port.mk>
