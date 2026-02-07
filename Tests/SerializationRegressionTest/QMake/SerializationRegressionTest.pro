TARGET = SerializationRegressionTest

include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR
LIBS += -lipackage -licomp -listd -liser -limath -li2d -li3d -licmm -liprm -lifile -liimg -libase -limod -litest

QT += core testlib

CONFIG += console

# Set configuration of custom builds:
# ARX Compiler:
ARXC_CONFIG = $$PWD/../../../Config/AcfTestConfig.awc
ARXC_FILES += $$files($$_PRO_FILE_PWD_/../*.acc, false)
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

INCLUDEPATH += $$OUT_PWD/$$AUXINCLUDEPATH

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)
include(../../../Config/QMake/CustomBuild.pri)
