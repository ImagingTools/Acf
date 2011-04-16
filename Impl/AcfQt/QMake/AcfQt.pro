TARGET = AcfQt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge
CONFIG += create_prl link_prl

CONFIG(debug, debug|release){
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Lib/ReleaseQMake
}

LIBS += -L$$DESTDIR -liqt -liqt2d -liqtdoc -liqtgui -liqtprm -liqtproc -liqtmm

INCLUDEPATH += ../../ 

SOURCES += ../Empty.cpp
