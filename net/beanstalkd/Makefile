PORTNAME=	beanstalkd
PORTVERSION=	1.12
PORTREVISION=	1
DISTVERSIONPREFIX=	v
CATEGORIES=	net

MAINTAINER=	vsevolod@FreeBSD.org
COMMENT=	Fast, distributed, in-memory workqueue service
WWW=		http://xph.us/software/beanstalkd/

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

USES=		gmake
USE_GITHUB=	yes
USE_RC_SUBR=	beanstalkd
TEST_TARGET=	check

GH_ACCOUNT=	beanstalkd

MAKE_ARGS+=	CC="${CC}" CFLAGS="${CFLAGS}"

PLIST_FILES=	bin/beanstalkd share/man/man1/beanstalkd.1.gz
SUB_LIST+=	RC_SUBR_SUFFIX=${RC_SUBR_SUFFIX}

PORTDOCS=	protocol.txt

OPTIONS_DEFINE=	DOCS

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/beanstalkd ${STAGEDIR}${PREFIX}/bin
	${INSTALL_MAN} ${WRKSRC}/doc/${PORTNAME}.1 \
		${STAGEDIR}${PREFIX}/share/man/man1

do-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
.for i in ${PORTDOCS}
	${INSTALL_DATA} ${WRKSRC}/doc/${i} ${STAGEDIR}${DOCSDIR}
.endfor

.include <bsd.port.mk>
