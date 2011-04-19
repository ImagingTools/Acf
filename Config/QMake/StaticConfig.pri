# Standard settings for an ACF static library

include(GeneralConfig.pri)

TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl link_prl

DESTDIR = ../../../Lib/$$COMPILER_DIR

