CONFIG += lib_bundle

INCLUDEPATH = ../../../Include

CONFIG(debug, debug|release) {
	LIBS += -L../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	LIBS += -L../../../Lib/ReleaseQMake
}
LIBS += AcfStd AcfQt icmpstr
