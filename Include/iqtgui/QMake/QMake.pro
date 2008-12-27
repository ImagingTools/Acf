TARGET = iqtgui
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

HEADERS += ../CApplicationComp.h
HEADERS += ../CDockWidgetGuiComp.h
HEADERS += ../CFileDialogLoaderComp.h
HEADERS += ../CFreeWidgetGuiComp.h
HEADERS += ../CGuiComponentBase.h
HEADERS += ../CHierarchicalCommand.h
HEADERS += ../CIconProviderComp.h
HEADERS += ../CLogGuiComp.h
HEADERS += ../CLoginGuiComp.h
HEADERS += ../CSelectionParamGuiComp.h
HEADERS += ../CSplashScreenGuiComp.h
HEADERS += ../CTabContainerGuiComp.h
HEADERS += ../CTreeWidgetFilter.h
HEADERS += ../CXmlFileReadArchive.h
HEADERS += ../CXmlFileWriteArchive.h
HEADERS += ../IDockManager.h
HEADERS += ../IGuiObject.h
HEADERS += ../IIconProvider.h
HEADERS += ../IMainWindowComponent.h
HEADERS += ../IQImageProvider.h
HEADERS += ../iqtgui.h
HEADERS += ../IToolBarManager.h
HEADERS += ../TDesignerGuiCompBase.h
HEADERS += ../TDesignerGuiObserverCompBase.h
HEADERS += ../TGuiComponentBase.h
HEADERS += ../TGuiObserverWrap.h
SOURCES += ../CApplicationComp.cpp
SOURCES += ../CDockWidgetGuiComp.cpp
SOURCES += ../CFileDialogLoaderComp.cpp
SOURCES += ../CFreeWidgetGuiComp.cpp
SOURCES += ../CGuiComponentBase.cpp
SOURCES += ../CHierarchicalCommand.cpp
SOURCES += ../CIconProviderComp.cpp
SOURCES += ../CLogGuiComp.cpp
SOURCES += ../CLoginGuiComp.cpp
SOURCES += ../CSelectionParamGuiComp.cpp
SOURCES += ../CSplashScreenGuiComp.cpp
SOURCES += ../CTabContainerGuiComp.cpp
SOURCES += ../CTreeWidgetFilter.cpp
FORMS += ../CLogGuiComp.ui
FORMS += ../CLoginGuiComp.ui
FORMS += ../CSelectionParamGuiComp.ui
FORMS += ../CSplashScreenGuiComp.ui
RESOURCES += ../iqtgui.qrc
