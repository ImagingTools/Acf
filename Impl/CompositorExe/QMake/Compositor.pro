include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Compositor

RESOURCES += ../*.qrc

LIBS += -licmpstr
LIBS += -lAcfLoc

QT += xml

ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

mac{
	ICON += ../Mac/Compositor.icns
}

include(../../../Config/QMake/CustomBuild.pri)

