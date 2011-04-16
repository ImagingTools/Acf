TARGET = Acf
TEMPLATE = app
CONFIG += link_prl

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

INCLUDEPATH += ../../../Include ../../../Impl

SOURCES += ../Main.cpp
