TARGET = SerializationRegressionTest

include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR
LIBS += -listd -liser -limath -li2d -li3d -licmm -litest

QT += core testlib

CONFIG += console

INCLUDEPATH += $$OUT_PWD/$$AUXINCLUDEPATH

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)
