PORTNAME=	enscript
PORTVERSION=	1.6.6
PORTREVISION?=	4	# ?= for chinese/enscript
CATEGORIES+=	print
MASTER_SITES=	GNU
PKGNAMESUFFIX=	-${PAPERSIZE}
DISTNAME=	enscript-${PORTVERSION}

MAINTAINER?=	mbeis@xs4all.nl
COMMENT=	ASCII to PostScript filter
WWW=		https://www.gnu.org/software/enscript/

USES=		cpe shebangfix perl5
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
CONFIGURE_ARGS=	--with-media=${PS}
SHEBANG_FILES=	scripts/diffpp.in scripts/sliceprint.in
perl_OLD_CMD=	@PERLPROG@

OPTIONS_DEFINE=	NLS
OPTIONS_SUB=	yes
NLS_USES=	gettext
NLS_CONFIGURE_ENABLE=	nls

CONFLICTS?=	enscript-a4 enscript-letterdj zh-enscript-a4

INFO=		enscript
CPE_VENDOR=	gnu

PS?=		Letter
PAPERSIZE?=	${PS:tl}

.if (${PAPERSIZE} != a4 && ${PAPERSIZE} != letter && ${PAPERSIZE} != letterdj)
.BEGIN:
	@${ECHO} "Error: invalid value for PAPERSIZE: \"${PAPERSIZE}\""
	@${ECHO} "Possible values are:  a4, letter, or letterdj"
	@${FALSE}
.endif

post-configure:
	${REINPLACE_CMD} -e 's|/usr/bin/perl|$${prefix}/bin/perl|g' ${WRKSRC}/scripts/Makefile

.include <bsd.port.mk>
