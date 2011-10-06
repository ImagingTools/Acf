include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Compositor

RESOURCES += ../*.qrc

LIBS += -licmpstr

QT += xml

ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

include(../../../Config/QMake/CustomBuild.pri)

