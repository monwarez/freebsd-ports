PORTNAME=	anndata
PORTVERSION=	0.10.5
DISTVERSIONSUFFIX=	.post1
CATEGORIES=	devel python
MASTER_SITES=	PYPI
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Annotated data
WWW=		https://anndata.readthedocs.io/en/latest/ \
		https://github.com/scverse/anndata

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}hatch-vcs>=0:devel/py-hatch-vcs@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}hatchling>=0:devel/py-hatchling@${PY_FLAVOR}
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}array-api-compat>=0:devel/py-array-api-compat@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}h5py>=3:science/py-h5py@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}natsort>=0:devel/py-natsort@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}numpy>=1.16.5,1:math/py-numpy@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}packaging>=20:devel/py-packaging@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pandas>=1.1.1,1:math/py-pandas@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}scipy>=1.4:science/py-scipy@${PY_FLAVOR}

USES=		python:3.9+
USE_PYTHON=	autoplist concurrent pep517

NO_ARCH=	yes

.include <bsd.port.pre.mk>

.if ${PYTHON_REL} < 31100
RUN_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}exceptiongroup>=0:devel/py-exceptiongroup@${PY_FLAVOR}
.endif

.include <bsd.port.post.mk>
