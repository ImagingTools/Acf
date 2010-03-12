TARGET = iqtgui
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Lib\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Lib\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

HEADERS += ../CAboutWidgetGuiComp.h
HEADERS += ../CApplicationComp.h
HEADERS += ../CComposedGuiComp.h
HEADERS += ../CDockWidgetGuiComp.h
HEADERS += ../CExtLineEdit.h
HEADERS += ../CFileDialogLoaderComp.h
HEADERS += ../CFilePersistenceGuiComp.h
HEADERS += ../CFreeWidgetGuiComp.h
HEADERS += ../CGuiComponentBase.h
HEADERS += ../CGuiComponentDialog.h
HEADERS += ../CHierarchicalCommand.h
HEADERS += ../CIconProviderComp.h
HEADERS += ../CItemDelegate.h
HEADERS += ../CLogGuiComp.h
HEADERS += ../CLoginGuiComp.h
HEADERS += ../CModelDialogGuiComp.h
HEADERS += ../CSelectionParamGuiComp.h
HEADERS += ../CSimpleMainWindowGuiComp.h
HEADERS += ../CSplashScreenGuiComp.h
HEADERS += ../CTabbedMultiViewGuiComp.h
HEADERS += ../CTabContainerGuiComp.h
HEADERS += ../CToolBarWidgetGuiComp.h
HEADERS += ../CTreeWidgetFilter.h
HEADERS += ../IDialog.h
HEADERS += ../IDropConsumer.h
HEADERS += ../IGuiApplication.h
HEADERS += ../IGuiObject.h
HEADERS += ../IIconProvider.h
HEADERS += ../IMainWindowComponent.h
HEADERS += ../iqtgui.h
HEADERS += ../TControlledWidgetWrap.h
HEADERS += ../TDesignerBasicGui.h
HEADERS += ../TDesignerGuiCompBase.h
HEADERS += ../TDesignerGuiObserverCompBase.h
HEADERS += ../TGuiComponentBase.h
HEADERS += ../TGuiObserverWrap.h
HEADERS += ../TRestorableGuiWrap.h
HEADERS += ../generated\ui_CFilePersistenceGuiComp.h
SOURCES += ../CAboutWidgetGuiComp.cpp
SOURCES += ../CApplicationComp.cpp
SOURCES += ../CComposedGuiComp.cpp
SOURCES += ../CDockWidgetGuiComp.cpp
SOURCES += ../CExtLineEdit.cpp
SOURCES += ../CFileDialogLoaderComp.cpp
SOURCES += ../CFilePersistenceGuiComp.cpp
SOURCES += ../CFreeWidgetGuiComp.cpp
SOURCES += ../CGuiComponentBase.cpp
SOURCES += ../CGuiComponentDialog.cpp
SOURCES += ../CHierarchicalCommand.cpp
SOURCES += ../CIconProviderComp.cpp
SOURCES += ../CItemDelegate.cpp
SOURCES += ../CLogGuiComp.cpp
SOURCES += ../CLoginGuiComp.cpp
SOURCES += ../CModelDialogGuiComp.cpp
SOURCES += ../CSelectionParamGuiComp.cpp
SOURCES += ../CSimpleMainWindowGuiComp.cpp
SOURCES += ../CSplashScreenGuiComp.cpp
SOURCES += ../CTabbedMultiViewGuiComp.cpp
SOURCES += ../CTabContainerGuiComp.cpp
SOURCES += ../CToolBarWidgetGuiComp.cpp
SOURCES += ../CTreeWidgetFilter.cpp
SOURCES += ../generated\moc_CExtLineEdit.cpp
SOURCES += ../generated\moc_CFilePersistenceGuiComp.cpp
SOURCES += ../generated\moc_CSimpleMainWindowGuiComp.cpp
FORMS += ../CAboutWidgetGuiComp.ui
FORMS += ../CFilePersistenceGuiComp.ui
FORMS += ../CLogGuiComp.ui
FORMS += ../CLoginGuiComp.ui
FORMS += ../CSelectionParamGuiComp.ui
FORMS += ../CSplashScreenGuiComp.ui
RESOURCES += ../iqtgui.qrc
