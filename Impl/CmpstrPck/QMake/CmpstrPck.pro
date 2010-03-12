TARGET = CmpstrPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(QScintilla)/Lib/DebugQMake 
	LIBS += AcfStd AcfQt 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(QScintilla)/Lib/ReleaseQMake 
	LIBS += AcfStd AcfQt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 


INCLUDEPATH += ../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" 

HEADERS += ../CmpstrPck.h
SOURCES += ../CmpstrPck.cpp
