include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = CompositorMini

LIBS += -licmpstr

SOURCES += ../Main.cpp
