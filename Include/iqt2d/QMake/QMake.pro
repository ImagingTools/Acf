TARGET = iqt2d
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

HEADERS += ../CAnnulusParamsGuiComp.h
HEADERS += ../CAnnulusSegmentParamsGuiComp.h
HEADERS += ../CAnnulusSegmentShape.h
HEADERS += ../CAnnulusShape.h
HEADERS += ../CCircleParamsGuiComp.h
HEADERS += ../CCircleShape.h
HEADERS += ../CGripShape.h
HEADERS += ../CImageShape.h
HEADERS += ../CImageViewComp.h
HEADERS += ../CLine2dParamsGuiComp.h
HEADERS += ../CLine2dShape.h
HEADERS += ../CPosition2dShape.h
HEADERS += ../CRectangleParamsGuiComp.h
HEADERS += ../CRectangleShape.h
HEADERS += ../CSceneConnectorGuiComp.h
HEADERS += ../CSceneProviderComp.h
HEADERS += ../iqt2d.h
HEADERS += ../ISceneExtender.h
HEADERS += ../ISceneProvider.h
HEADERS += ../TObjectShapeBase.h
HEADERS += ../TSceneExtenderCompBase.h
HEADERS += ../TShapeBase.h
SOURCES += ../CAnnulusParamsGuiComp.cpp
SOURCES += ../CAnnulusSegmentParamsGuiComp.cpp
SOURCES += ../CAnnulusSegmentShape.cpp
SOURCES += ../CAnnulusShape.cpp
SOURCES += ../CCircleParamsGuiComp.cpp
SOURCES += ../CCircleShape.cpp
SOURCES += ../CGripShape.cpp
SOURCES += ../CImageShape.cpp
SOURCES += ../CImageViewComp.cpp
SOURCES += ../CLine2dParamsGuiComp.cpp
SOURCES += ../CLine2dShape.cpp
SOURCES += ../CPosition2dShape.cpp
SOURCES += ../CRectangleParamsGuiComp.cpp
SOURCES += ../CRectangleShape.cpp
SOURCES += ../CSceneConnectorGuiComp.cpp
SOURCES += ../CSceneProviderComp.cpp
FORMS += ../CAnnulusParamsGuiComp.ui
FORMS += ../CAnnulusSegmentParamsGuiComp.ui
FORMS += ../CCircleParamsGuiComp.ui
FORMS += ../CLine2dParamsGuiComp.ui
FORMS += ../CRectangleParamsGuiComp.ui
FORMS += ../CSceneConnectorGuiComp.ui
