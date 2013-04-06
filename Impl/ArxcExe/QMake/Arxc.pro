include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

mac*{
	CONFIG -= app_bundle
}

TARGET = Arxc

HEADERS =

LIBS += -lipackage

include(../../../Config/QMake/AcfStd.pri)
include(../../../Config/QMake/AcfQt.pri)
