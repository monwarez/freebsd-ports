# $FreeBSD$

PORTNAME=	redis-actionpack
PORTVERSION=	5.0.2
CATEGORIES=	databases rubygems
MASTER_SITES=	RG
PKGNAMESUFFIX=	-rails52

MAINTAINER=	ruby@FreeBSD.org
COMMENT=	Redis session store for ActionPack

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/MIT-LICENSE

RUN_DEPENDS=	rubygem-actionpack52>0:www/rubygem-actionpack52 \
		rubygem-redis-rack>=1:www/rubygem-redis-rack \
		rubygem-redis-store>=1.1.0:devel/rubygem-redis-store

USES=		gem
USE_RUBY=	yes

NO_ARCH=	yes

.include <bsd.port.mk>
