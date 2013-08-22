include(../../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = DocViewSimu

DESTDIR = ../../../../../Bin/$$COMPILER_DIR

LIBS += -lAcfStd -lAcfQt

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/DocViewSimu

QT += main core gui


INCLUDEPATH += ../.. ../../../../../Impl "$(QScintilla)/include"

