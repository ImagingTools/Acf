#Standard settings for an ACF package project

include (GeneralConfig.pri)

TEMPLATE = lib

TARGET_EXT = .arp

CONFIG += dll
CONFIG += link_prl

DESTDIR = ../../../Bin/$$COMPILER_DIR

!win32-msvc*{
	QMAKE_CFLAGS += -fPIC
	QMAKE_EXTENSION_SHLIB = arp
}
