# $FreeBSD$

PORTNAME=	webpacker
PORTVERSION=	4.2.2
CATEGORIES=	devel rubygems
MASTER_SITES=	RG
PKGNAMESUFFIX=	-rails5

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Use Webpack to manage app-like JavaScript modules in Rails

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/MIT-LICENSE

RUN_DEPENDS=	rubygem-activesupport5>=4.2:devel/rubygem-activesupport5 \
		rubygem-rack-proxy>=0.6.1:www/rubygem-rack-proxy \
		rubygem-railties5>=4.2:www/rubygem-railties5 \
		yarn:www/yarn

USES=		gem
USE_RUBY=	yes

NO_ARCH=	yes

# webpacker 5.0.0+ requires Rails 5.2.0+
PORTSCOUT=	limit:^4\.

.include <bsd.port.mk>
