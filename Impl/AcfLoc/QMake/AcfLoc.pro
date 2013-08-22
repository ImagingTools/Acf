include(../../../Config/QMake/StaticConfig.pri)

TARGET = AcfLoc


HEADERS =
SOURCES =
ACF_TRANSLATIONS += ../*.ts
ACF_TRANSLATIONS_OUTDIR = ../../GeneratedFiles/AcfLoc
RESOURCES += ../*.qrc

include(../../../Config/QMake/CustomBuild.pri)
