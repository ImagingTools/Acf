TARGET = iqtprm

include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/QtGuiBaseConfig.pri)

QT += uitools

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, false)

