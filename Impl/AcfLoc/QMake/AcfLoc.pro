TARGET = AcfLoc

include(../../../Config/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
RESOURCES += ../*.qrc

ACF_TRANSLATIONS += ../*.ts
ACF_TRANSLATIONS_OUTDIR = ../../GeneratedFiles/AcfLoc

include(../../../Config/QMake/CustomBuild.pri)
