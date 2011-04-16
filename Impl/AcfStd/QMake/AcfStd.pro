TARGET = AcfStd
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

LIBS += -L$$DESTDIR -libase -licomp -li2d -liproc -lidoc -liimg -lisec -liprm -limod -licmm -limath -liser -lisys -listd

INCLUDEPATH += ../../

SOURCES += ../Empty.cpp
