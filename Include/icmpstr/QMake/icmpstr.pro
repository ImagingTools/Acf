TARGET = icmpstr
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

HEADERS += ../CAttributeEditorComp.h
HEADERS += ../CComponentConnector.h
HEADERS += ../CComponentHelpFileProviderComp.h
HEADERS += ../CGraphicsConnectorItem.h
HEADERS += ../CPackageOverviewComp.h
HEADERS += ../CRegistryConsistInfoComp.h
HEADERS += ../CRegistryElementShape.h
HEADERS += ../CRegistryLoaderComp.h
HEADERS += ../CRegistryPreviewComp.h
HEADERS += ../CRegistryPropEditorComp.h
HEADERS += ../CVisualRegistryComp.h
HEADERS += ../CVisualRegistryElement.h
HEADERS += ../CVisualRegistryScenographerComp.h
HEADERS += ../IAttributeSelectionObserver.h
HEADERS += ../icmpstr.h
HEADERS += ../IComponentNote.h
HEADERS += ../IElementSelectionInfo.h
HEADERS += ../IRegistryConsistInfo.h
HEADERS += ../IRegistryEditController.h
HEADERS += ../IRegistryPreview.h
HEADERS += ../generated\ui_CAttributeEditorComp.h
HEADERS += ../generated\ui_CPackageOverviewComp.h
HEADERS += ../generated\ui_CRegistryPropEditorComp.h
SOURCES += ../CAttributeEditorComp.cpp
SOURCES += ../CComponentHelpFileProviderComp.cpp
SOURCES += ../CGraphicsConnectorItem.cpp
SOURCES += ../CPackageOverviewComp.cpp
SOURCES += ../CRegistryConsistInfoComp.cpp
SOURCES += ../CRegistryElementShape.cpp
SOURCES += ../CRegistryLoaderComp.cpp
SOURCES += ../CRegistryPreviewComp.cpp
SOURCES += ../CRegistryPropEditorComp.cpp
SOURCES += ../CVisualRegistryComp.cpp
SOURCES += ../CVisualRegistryElement.cpp
SOURCES += ../CVisualRegistryScenographerComp.cpp
SOURCES += ../generated\moc_CAttributeEditorComp.cpp
SOURCES += ../generated\moc_CGraphicsConnectorItem.cpp
SOURCES += ../generated\moc_CPackageOverviewComp.cpp
SOURCES += ../generated\moc_CRegistryElementShape.cpp
SOURCES += ../generated\moc_CRegistryPreviewComp.cpp
SOURCES += ../generated\moc_CRegistryPropEditorComp.cpp
SOURCES += ../generated\moc_CVisualRegistryScenographerComp.cpp
SOURCES += ../generated\qrc_icmpstr.cpp
FORMS += ../CAttributeEditorComp.ui
FORMS += ../CPackageOverviewComp.ui
FORMS += ../CRegistryPropEditorComp.ui
RESOURCES += ../icmpstr.qrc
