TARGET = ComposedGuiTest

include(../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../Config/QMake/QtBaseConfig.pri)

LIBS += -L../../../../Lib/$$COMPILER_DIR
LIBS += -listd -liser -licomp -liwidgets -lAcfLoc

QT += core gui

# Set configuration of custom builds:
# ARX Compiler:
ARXC_CONFIG = $$PWD/../../../../Config/AcfConfig.awc
ARXC_FILES += $$files($$_PRO_FILE_PWD_/../*.acc, false)
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

INCLUDEPATH += $$OUT_PWD/$$AUXINCLUDEPATH

include(../../../../Config/QMake/AcfQt.pri)
include(../../../../Config/QMake/AcfStdGui.pri)
include(../../../../Config/QMake/AcfStd.pri)
include(../../../../Config/QMake/CustomBuild.pri)

