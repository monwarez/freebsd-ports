PORTNAME=	ktuberling
DISTVERSION=	${KDE_APPLICATIONS_VERSION}
CATEGORIES=	games kde kde-applications

MAINTAINER=	kde@FreeBSD.org
COMMENT=	${${PORTNAME:tu}_DESC}
WWW=		https://www.kde.org/applications/games/ktuberling/

USES=		cmake compiler:c++11-lang desktop-file-utils gettext kde:5 qt:5 tar:xz
USE_KDE=	auth bookmarks codecs completion config configwidgets \
		coreaddons crash dbusaddons emoticons guiaddons i18n \
		iconthemes init itemmodels itemviews jobwidgets \
		kdelibs4support kio libkdegames newstuff notifications \
		notifyconfig parts phonon service solid sonnet textwidgets \
		unitconversion widgetsaddons windowsystem xmlgui \
		ecm:build
USE_QT=		concurrent core dbus declarative gui multimedia network \
		printsupport svg testlib widgets xml \
		buildtools:build qmake:build

OPTIONS_DEFINE=	DOCS

.include <${.CURDIR}/../kdegames/Makefile.common>
.include <bsd.port.mk>
