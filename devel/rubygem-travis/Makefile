PORTNAME=	travis
PORTVERSION=	1.11.1
PORTREVISION=	1
CATEGORIES=	devel rubygems
MASTER_SITES=	RG

MAINTAINER=	ruby@FreeBSD.org
COMMENT=	CLI and Ruby client library for Travis CI
WWW=		https://github.com/travis-ci/travis.rb

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	rubygem-faraday1>=1.0<2:www/rubygem-faraday1 \
		rubygem-faraday_middleware>=1.0<2:www/rubygem-faraday_middleware \
		rubygem-gh>=0.13<1:devel/rubygem-gh \
		rubygem-highline2>=2.0<3:devel/rubygem-highline2 \
		rubygem-json_pure>=2.3<3:devel/rubygem-json_pure \
		rubygem-launchy>=2.1<3:devel/rubygem-launchy \
		rubygem-pusher-client>=0.4<1:www/rubygem-pusher-client

USES=		gem

NO_ARCH=	yes

PLIST_FILES=	bin/travis

.include <bsd.port.mk>
