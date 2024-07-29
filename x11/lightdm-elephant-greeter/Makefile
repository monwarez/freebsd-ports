PORTNAME=	lightdm-elephant-greeter
DISTVERSION=	v20210518
CATEGORIES=	x11

MAINTAINER=	monwarez@mailoo.org
COMMENT=	A small and simple LightDM greeter using Python and GTK
WWW=		https://github.com/max-moser/lightdm-elephant-greeter

USE_GITHUB=	yes
GH_ACCOUNT=	max-moser
GH_TAGNAME=	ee570db

NO_BUILD=	yes

USE=	gmake

STRIP=

do-configure:
	@${CP} ${WRKSRC}/elephant-greeter.conf.base ${WRKSRC}/elephant-greeter.conf
	@${REINPLACE_CMD} -e 's|INSTALL_PATH|${LOCALBASE}|' ${WRKSRC}/elephant-greeter.conf

do-install:
	@${MKDIR} ${STAGEDIR}${PREFIX}/etc/lightdm
	@${MKDIR} ${STAGEDIR}${PREFIX}/share/lightdm/greeters
	@${MKDIR} ${STAGEDIR}${PREFIX}/share/elephant-greeter

	${INSTALL_DATA} ${WRKSRC}/elephant-greeter.conf ${STAGEDIR}${PREFIX}/etc/lightdm
	${INSTALL_PROGRAM} ${WRKSRC}/elephant-greeter.py ${STAGEDIR}${PREFIX}/bin
	${INSTALL_DATA} ${WRKSRC}/lightdm-elephant-greeter.desktop ${STAGEDIR}${PREFIX}/share/lightdm/greeters
	${INSTALL_DATA} ${WRKSRC}/lightdm-elephant-greeter-x11.desktop ${STAGEDIR}${PREFIX}/share/lightdm/greeters
	${INSTALL_DATA} ${WRKSRC}/elephant-greeter.ui ${STAGEDIR}${PREFIX}/share/elephant-greeter
	cd ${WRKSRC} && ${COPYTREE_SHARE} img ${STAGEDIR}${PREFIX}/share/elephant-greeter

.include <bsd.port.mk>

