TARGET = Compositor
TEMPLATE = app

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake 
	LIBS += AcfStd iqt 
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake 
	LIBS += AcfStd iqt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui xml sql network 


INCLUDEPATH += "$(ACFDIR)/Include" "$(ACFDIR)/Impl" "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" .. 

HEADERS += ../CAttributeEditorComp.h
HEADERS += ../CComponentConnector.h
HEADERS += ../CComponentNote.h
HEADERS += ../CComponentNoteView.h
HEADERS += ../CComponentView.h
HEADERS += ../CPackageOverviewComp.h
HEADERS += ../CRegistryLoaderComp.h
HEADERS += ../CRegistryModelComp.h
HEADERS += ../CRegistryPreviewComp.h
HEADERS += ../CRegistryView.h
HEADERS += ../CRegistryViewComp.h
HEADERS += ../IAttributeSelectionObserver.h
HEADERS += ../IComponentNote.h
HEADERS += ../IRegistryPreview.h
SOURCES += ../CAttributeEditorComp.cpp
SOURCES += ../CComponentConnector.cpp
SOURCES += ../CComponentNote.cpp
SOURCES += ../CComponentNoteView.cpp
SOURCES += ../CComponentView.cpp
SOURCES += ../CPackageOverviewComp.cpp
SOURCES += ../CRegistryLoaderComp.cpp
SOURCES += ../CRegistryModelComp.cpp
SOURCES += ../CRegistryPreviewComp.cpp
SOURCES += ../CRegistryView.cpp
SOURCES += ../CRegistryViewComp.cpp
SOURCES += ../Main.cpp
SOURCES += ../generated\moc_CRegistryView.cpp
FORMS += ../CAttributeEditorComp.ui
FORMS += ../CPackageOverviewComp.ui
RESOURCES += ../Compositor.qrc
