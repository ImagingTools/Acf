TARGET = TutorialPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

debug {
	DESTDIR = ../../../../../Bin/DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake 
	LIBS += AcfStd AcfQt 
}
release {
	DESTDIR = ../../../../../Bin/ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake 
	LIBS += AcfStd AcfQt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui xml sql network 


INCLUDEPATH += ../.. ../../../../../Include 

HEADERS += ../TutorialPck.h
SOURCES += ../Main.cpp
