PORTNAME=	arti
DISTVERSION=	1.1.13
PORTREVISION=	1
CATEGORIES=	security

MAINTAINER=	cs@FreeBSD.org
COMMENT=	Implementation of Tor, in Rust
WWW=		https://arti.torproject.org/

LICENSE=	MIT

NOT_FOR_ARCHS=	i386
NOT_FOR_ARCHS_REASON=Zeroize crate does not support i386
LIB_DEPENDS=	libzstd.so:archivers/zstd

USES=		cargo sqlite ssl
USE_RC_SUBR=	arti
GROUPS=		_arti
USERS=		_arti
SUB_LIST=	USERS="${USERS}" GROUPS="${GROUPS}"
PLIST_SUB=	USERS="${USERS}" GROUPS="${GROUPS}"
USE_GITLAB=	yes
GL_SITE=	https://gitlab.torproject.org
GL_ACCOUNT=	tpo/core
GL_TAGNAME=	3ef803627bc84e6ba8c89ca57128141f237f35f8
CARGO_FEATURES=	tokio rustls
CARGO_INSTALL=	no

do-install:
	${INSTALL_SCRIPT} ${CARGO_TARGET_DIR}/${CARGO_BUILD_TARGET}/*/arti ${STAGEDIR}${PREFIX}/bin/arti

post-install:
	${STRIP_CMD} ${STAGEDIR}${PREFIX}/bin/arti
	@${MKDIR} ${STAGEDIR}${ETCDIR} ${STAGEDIR}/var/log/arti ${STAGEDIR}/var/run/arti ${STAGEDIR}/var/db/arti
	${INSTALL_DATA} ${WRKSRC}/crates/arti/src/arti-example-config.toml ${STAGEDIR}${ETCDIR}/arti.toml.sample

.include <bsd.port.mk>
