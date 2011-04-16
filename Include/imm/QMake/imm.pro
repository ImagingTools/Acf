TARGET = imm
TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

CONFIG(debug, debug|release){
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

INCLUDEPATH += ../..

HEADERS += ../*.h
SOURCES += ../*.cpp
