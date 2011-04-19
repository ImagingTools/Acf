include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Arxc

LIBS += -licmpstr

SOURCES += ../Main.cpp
