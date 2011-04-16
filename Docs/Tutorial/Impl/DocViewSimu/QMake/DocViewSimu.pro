TARGET = DocViewSimu_exe
TEMPLATE = app

CONFIG(debug, debug|release){
	DESTDIR = ../../../../../Bin/DebugQMake
	LIBS += -L../../../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../../../Bin/ReleaseQMake
	LIBS += -L../../../../../Lib/ReleaseQMake
}

LIBS += -lAcfStd -lAcfQt

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 


INCLUDEPATH += ../.. ../../../../../Include ../../../../../Impl "$(QScintilla)/include"

SOURCES += ../*.cpp
