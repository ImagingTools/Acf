CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge

CONFIG(debug, debug|release) {
	LIBDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	LIBDIR = ../../../Lib/ReleaseQMake
}
OBJECTS += $$LIBDIR/i2d.lib
OBJECTS += $$LIBDIR/ibase.lib
OBJECTS += $$LIBDIR/icmm.lib
OBJECTS += $$LIBDIR/icomp.lib
OBJECTS += $$LIBDIR/idoc.lib
OBJECTS += $$LIBDIR/iimg.lib
OBJECTS += $$LIBDIR/imath.lib
OBJECTS += $$LIBDIR/imod.lib
OBJECTS += $$LIBDIR/iprm.lib
OBJECTS += $$LIBDIR/isec.lib
OBJECTS += $$LIBDIR/iser.lib
OBJECTS += $$LIBDIR/istd.lib
OBJECTS += $$LIBDIR/isys.lib
OBJECTS += $$LIBDIR/iproc.lib
