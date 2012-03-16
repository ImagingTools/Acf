include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../../Config/QMake/AcfStd.pri)
include(../../../Config/QMake/AcfQt.pri)

mac*{
	CONFIG -= app_bundle
}

TARGET = Acf

HEADERS =

LIBS += -licmpstr

QT += xml
