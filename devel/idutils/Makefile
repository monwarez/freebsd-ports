PORTNAME=	idutils
PORTVERSION=	4.6
PORTREVISION=	6
CATEGORIES=	devel
MASTER_SITES=	GNU

MAINTAINER=	peterjeremy@acm.org
COMMENT=	The classic Berkeley gid/lid tools for looking up variables in code
WWW=		https://www.gnu.org/software/idutils/

LICENSE=	GPLv3

OPTIONS_DEFINE=	NLS

USES=		charsetfix tar:xz uniquefiles
# Also see patch-src_defid and patch-lisp_idutils.el
UNIQUE_PREFIX=	g
UNIQUE_PREFIX_FILES=	bin/gid \
			share/man/man1/gid.1

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
CONFIGURE_ENV=	EMACS='no'
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

INFO=		idutils

NLS_USES=	gettext
NLS_CONFIGURE_ENABLE=	nls

OPTIONS_SUB=	yes

post-install:
	${INSTALL_DATA} ${WRKSRC}/lisp/idutils.el ${STAGEDIR}${PREFIX}/share/emacs/site-lisp

.include <bsd.port.mk>
