PORTNAME=	PyQt-builder
PORTVERSION=	${PYQTBUILDER_VERSION}
CATEGORIES=	devel python
MASTER_SITES=	${MASTER_SITES_PYQTBUILDER}
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	kde@FreeBSD.org
COMMENT=	The PEP 517 compliant PyQt build system
WWW=		https://www.riverbankcomputing.com/software/pyqt-builder/

LICENSE=		SIP GPLv2 GPLv3
LICENSE_COMB=		multi
LICENSE_NAME_SIP=	SIP License
LICENSE_GROUPS_SIP=	FSF GPL OSI
LICENSE_PERMS_SIP=	dist-mirror dist-sell pkg-mirror pkg-sell auto-accept
LICENSE_FILE_SIP=	${WRKSRC}/LICENSE
LICENSE_FILE_GPLv2=	${WRKSRC}/LICENSE-GPL2
LICENSE_FILE_GPLv3=	${WRKSRC}/LICENSE-GPL3

BUILD_DEPENDS=	${PY_SETUPTOOLS} \
		${PYTHON_PKGNAMEPREFIX}wheel>0:devel/py-wheel@${PY_FLAVOR}

USES=		python pyqt:5
USE_PYTHON=	autoplist concurrent pep517
USE_PYQT=	#

NO_ARCH=	yes

.include <bsd.port.mk>
