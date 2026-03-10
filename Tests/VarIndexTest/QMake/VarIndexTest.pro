TARGET = VarIndexTest

include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR
LIBS += -listd -litest

QT += core testlib

CONFIG += console

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)
