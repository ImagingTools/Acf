TARGET = BasePck
TEMPLATE = lib

CONFIG += dll
CONFIG += link_prl

include(../../AcfStd/QMake/AcfStd.pri)

TARGET_EXT = .arp

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake 
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake 
}

win32-msvc*{
	LIBS += -lAcfStd
}
else{
	LIBS += $$ACFSTD_LIBS
}

QT -= gui core

INCLUDEPATH += ../../../Include
HEADERS += ../BasePck.h
SOURCES += ../BasePck.cpp
