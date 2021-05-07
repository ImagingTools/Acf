TARGET = iqt2d

include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/QtGuiBaseConfig.pri)

RESOURCES += $$files($$_PRO_FILE_PWD_/../*.qrc, true)
FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, true)

