greaterThan(QT_MAJOR_VERSION, 4) {
TEMPLATE = subdirs
CONFIG += ordered

# ensure Makefile with debug and release configuration on Mac
CONFIG += x86
CONFIG += debug_and_release

SUBDIRS +=	AcfLocTrans.pro
SUBDIRS +=	AcfLocRcc.pro
}
else {
TARGET = AcfLoc

include(../../../Config/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
GENERATED_RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

ACF_TRANSLATIONS += $$_PRO_FILE_PWD_/../*.ts

include(../../../Config/QMake/CustomBuild.pri)
}