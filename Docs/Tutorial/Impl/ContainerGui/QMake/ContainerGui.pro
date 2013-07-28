include(../../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = ContainerGui

DESTDIR = ../../../../../Bin/$$COMPILER_DIR

LIBS += -lAcfStd -lAcfQt

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 


