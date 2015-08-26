# Standard settings for an ACF static library

include(GeneralConfig.pri)

TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl link_prl

INCLUDEPATH += ../../
DESTDIR = ../../../Lib/$$COMPILER_DIR

HEADERS += ../*.h
SOURCES += ../*.cpp

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}
