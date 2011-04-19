include(../../../Config/QMake/ComponentConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

LIBS += -licmpstr

include(../../AcfQt/QMake/AcfQt.pri)
include(../../AcfStd/QMake/AcfStd.pri)

TARGET = CmpstrPck

QT += xml
