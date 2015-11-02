include(../../../../Acf/Config/QMake/GeneralConfig.pri)

TEMPLATE = aux
TARGET = none

OBJECTS_DIR = ./
DLLDESTDIR = ./

HEADERS =
SOURCES =

ACF_TRANSLATIONS += $$_PRO_FILE_PWD_/../*.ts
ACF_TRANSLATIONS_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/AcfLoc

include(../../../../Acf/Config/QMake/CustomBuild.pri)
