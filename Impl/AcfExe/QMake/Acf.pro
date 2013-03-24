include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

mac*{
	CONFIG -= app_bundle
}

TARGET = Acf

HEADERS =

LIBS += -licmpstr

QT += xml

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)
