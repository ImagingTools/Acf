TARGET = iqtdoc
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtSql" "$(QTDIR)/include/QtOpenGL" "$(QTDIR)/include/QtNetwork" "$(QTDIR)/include/ActiveQt" "$(QTDIR)/include/QtAssistant" "$(QTDIR)/include/QtSvg" 

HEADERS += ../CExtendedDocumentTemplateComp.h
HEADERS += ../CMainWindowGuiComp.h
HEADERS += ../CModelEditorGuiComp.h
HEADERS += ../CMultiDocumentWorkspaceGuiComp.h
HEADERS += ../iqtdoc.h
HEADERS += ../IWorkspaceController.h
SOURCES += ../CExtendedDocumentTemplateComp.cpp
SOURCES += ../CMainWindowGuiComp.cpp
SOURCES += ../CModelEditorGuiComp.cpp
SOURCES += ../CMultiDocumentWorkspaceGuiComp.cpp
