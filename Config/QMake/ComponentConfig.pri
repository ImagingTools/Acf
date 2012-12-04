#Standard settings for an ACF package project

include (GeneralConfig.pri)

INCLUDEPATH += ../../../Include

TEMPLATE = lib

TARGET_EXT = .arp

CONFIG += link_prl

DESTDIR = ../../../Bin/$$COMPILER_DIR

!win32-msvc*{
	CONFIG += plugin
	QMAKE_CFLAGS += -fPIC
	QMAKE_EXTENSION_SHLIB = arp
	QMAKE_POST_LINK = mv $$OUT_PWD/../../../Bin/$$COMPILER_DIR/lib$$TARGET$$TARGET_EXT $$OUT_PWD/../../../Bin/$$COMPILER_DIR/$$TARGET$$TARGET_EXT

}
else{
	CONFIG += dll
}
