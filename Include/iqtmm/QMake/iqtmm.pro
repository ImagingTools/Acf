TARGET = iqtmm
TEMPLATE = lib
CONFIG += staticlib
CONFIG += stl
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

QT += core gui xml svg phonon

INCLUDEPATH += ../..

HEADERS += ../*.h
SOURCES += ../*.cpp
FORMS += ../*.ui
