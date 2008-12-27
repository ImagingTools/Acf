TARGET = LogSimu_exe
TEMPLATE = app

debug {
	DESTDIR = ../../../../../Bin/DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake 
	LIBS += AcfStd AcfQt 
}
release {
	DESTDIR = ../../../../../Bin/ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake 
	LIBS += AcfStd AcfQt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 


INCLUDEPATH += .. "$(ACFDIR)/Include" "$(ACFDIR)/Impl" ../.. "$(QScintilla)/include" 

HEADERS += ../CLogClientGuiComp.h
SOURCES += ../CLogClientGuiComp.cpp
SOURCES += ../Main.cpp
