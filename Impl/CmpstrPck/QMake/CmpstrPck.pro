TARGET = CmpstrPck
TEMPLATE = lib

CONFIG += dll
CONFIG += link_prl

TARGET_EXT = .arp

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake 
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake 
}

LIBS += -licmpstr -lAcfQt -lAcfStd

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += core gui xml

INCLUDEPATH += ../../../Include

HEADERS += ../CmpstrPck.h
SOURCES += ../CmpstrPck.cpp
