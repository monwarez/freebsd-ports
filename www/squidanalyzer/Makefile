PORTNAME=	squidanalyzer
PORTVERSION=	6.5
PORTREVISION=	1
DISTVERSIONPREFIX=	v
CATEGORIES=	www perl5

MAINTAINER=	dominique.goncalves@gmail.com
COMMENT=	Squid access log report generation tool
WWW=		https://squidanalyzer.darold.net/

LICENSE=	GPLv3

USE_GITHUB=	yes
GH_ACCOUNT=	darold

MAKE_JOBS_UNSAFE=yes

USE_PERL5=	configure
USES=		perl5
CONFIGURE_ENV=	DESTDIR=${STAGEDIR} \
		LOGFILE=/var/log/squid3/access.log \
		BINDIR=${PREFIX}/bin \
		CONFDIR=${PREFIX}/etc/squidreport \
		HTMLDIR=${PREFIX}/www/squidreport \
		BASEURL=/squidreport \
		MANDIR=${PREFIX}/share/man/man3 \
		DOCDIR=${PREFIX}/share/doc/squidanalyzer \
		INSTALLDIRS=site
ETCDIR=		${PREFIX}/etc/squidreport
SUB_FILES=	pkg-message

CONF_FILES=	excluded included network-aliases \
		squidanalyzer.conf user-aliases

OPTIONS_DEFINE=	DOCS

post-install:
.for file in ${CONF_FILES}
	@${MV} ${STAGEDIR}${ETCDIR}/${file} \
		${STAGEDIR}${ETCDIR}/${file}.sample
.endfor

.include <bsd.port.mk>
