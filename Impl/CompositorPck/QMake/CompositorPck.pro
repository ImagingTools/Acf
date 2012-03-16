TARGET = CompositorPck

include(../../../Config/QMake/ComponentConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

LIBS += -licmpstr

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)

QT += xml
