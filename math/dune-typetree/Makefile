PORTNAME=	dune-typetree
DISTVERSIONPREFIX=	v
DISTVERSION=	2.8.0
PORTREVISION=	2
CATEGORIES=	math

MAINTAINER=	yuri@FreeBSD.org
COMMENT=	DUNE framework: The TypeTree library
WWW=		https://www.dune-project.org/

LICENSE=	GPLv2
LICENSE_FILE=	${WRKSRC}/LICENSE.md

BUILD_DEPENDS=	dune-common>0:math/dune-common \
		openmpi4>0:net/openmpi4 \
		vc>0:devel/vc
RUN_DEPENDS=	dune-common>0:math/dune-common \
		openmpi4>0:net/openmpi4 \
		vc>0:devel/vc

USES=		cmake compiler:c++17-lang fortran pkgconfig
USE_GITLAB=	yes

GL_SITE=	https://gitlab.dune-project.org
GL_ACCOUNT=	staging
GL_TAGNAME=	cf6b5b4d3abe5dfa748815a69f6a8bda02af36f0

NO_ARCH=	yes

OPTIONS_DEFINE=	DOCS
OPTIONS_SUB=	yes

DOCS_CMAKE_BOOL=	FREEBSD_INSTALL_DOCS
DOCS_BUILD_DEPENDS=	doxygen:devel/doxygen \
			sphinx-build:textproc/py-sphinx

PORTDOCS=	*

.include <bsd.port.mk>
