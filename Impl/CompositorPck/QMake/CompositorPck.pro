TARGET = CompositorPck

include(../../../Config/QMake/ComponentConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

LIBS += -licmpstr

QT += xml

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)
