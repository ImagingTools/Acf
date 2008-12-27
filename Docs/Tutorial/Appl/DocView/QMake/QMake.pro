TARGET = DocView
TEMPLATE = lib
CONFIG += staticlib

debug {
	DESTDIR = ../../../../../Lib/DebugQMake
}
release {
	DESTDIR = ../../../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += .. ../../../../../Include "$(ACFDIR)/Include" 

HEADERS += ../CTextEditorComp.h
SOURCES += ../CTextEditorComp.cpp
