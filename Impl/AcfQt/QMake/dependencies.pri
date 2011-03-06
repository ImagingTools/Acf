CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge

CONFIG(debug, debug|release) {
        LIBDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
        LIBDIR = ../../../Lib/ReleaseQMake
}

OBJECTS += $$LIBDIR/iqt.lib
OBJECTS += $$LIBDIR/iqt2d.lib
OBJECTS += $$LIBDIR/iqtdoc.lib
OBJECTS += $$LIBDIR/iqtgui.lib
OBJECTS += $$LIBDIR/iqtprm.lib
OBJECTS += $$LIBDIR/iqtproc.lib
OBJECTS += $$LIBDIR/iqtmm.lib
