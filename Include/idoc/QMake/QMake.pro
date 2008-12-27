TARGET = idoc
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ 

HEADERS += ../CCompositeDocumentTemplateComp.h
HEADERS += ../CDocumentManagerBase.h
HEADERS += ../CSingleDocumentTemplateBase.h
HEADERS += ../CSingleDocumentTemplateComp.h
HEADERS += ../ICommand.h
HEADERS += ../ICommandsProvider.h
HEADERS += ../idoc.h
HEADERS += ../IDocumentManager.h
HEADERS += ../IDocumentTemplate.h
SOURCES += ../CCompositeDocumentTemplateComp.cpp
SOURCES += ../CDocumentManagerBase.cpp
SOURCES += ../CSingleDocumentTemplateBase.cpp
SOURCES += ../CSingleDocumentTemplateComp.cpp
