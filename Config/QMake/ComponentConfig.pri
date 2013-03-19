#Standard settings for an ACF package project

include (GeneralConfig.pri)

INCLUDEPATH += ../../../Include

TEMPLATE = lib

TARGET_EXT = .arp

DESTDIR = ../../../Bin/$$COMPILER_DIR

CONFIG += link_prl

!win32-msvc*{
	CONFIG += plugin no_plugin_name_prefix
	QMAKE_CFLAGS += -fPIC
	QMAKE_EXTENSION_SHLIB = arp
#	QMAKE_POST_LINK = mv $$OUT_PWD/../../../Bin/$$COMPILER_DIR/lib$$TARGET$$TARGET_EXT $$OUT_PWD/../../../Bin/$$COMPILER_DIR/$$TARGET$$TARGET_EXT
}
else{
	CONFIG += dll
}

HEADERS += ../*.h
SOURCES += ../*.cpp
