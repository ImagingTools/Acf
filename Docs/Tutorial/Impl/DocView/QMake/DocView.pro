TARGET = DocView
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release){
	DESTDIR = ../../../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += core gui

INCLUDEPATH += .. ../../../../../Include

HEADERS += ../*.h
SOURCES += ../*.cpp
