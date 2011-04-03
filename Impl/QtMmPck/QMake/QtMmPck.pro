TARGET = QtMmPck
TEMPLATE = lib

CONFIG += dll
CONFIG += link_prl

TARGET_EXT = .arp

include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake 
	LIBS += -lphonond4
}

CONFIG(release, debug|release){
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake 
	LIBS += -lphonon4
}

win32-msvc*{
	LIBS += -lAcfQt -lAcfStd
}
else{
	LIBS += $$ACFQT_LIBS $$ACFSTD_LIBS
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += core gui svg

INCLUDEPATH += ../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/phonon" "$(QTDIR)/include/QtSvg" 

HEADERS += ../QtMmPck.h
SOURCES += ../QtMmPck.cpp
