include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtBase.config)

LIBS += -licmpstr

include(../../AcfQt/QMake/AcfQt.pri)
include(../../AcfStd/QMake/AcfStd.pri)

TARGET = CmpstrPck

QT += xml
