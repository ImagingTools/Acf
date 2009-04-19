TARGET = Compositor
TEMPLATE = app

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake 
	LIBS += AcfStd iqt 
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake 
	LIBS += AcfStd iqt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui xml sql network 

include(dependencies.pri)

INCLUDEPATH += "$(ACFDIR)/Include" "$(ACFDIR)/Impl" "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" .. 

SOURCES += ../Main.cpp
