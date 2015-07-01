#Standard settings for an ACF application

include (GeneralConfig.pri)

TEMPLATE = app

CONFIG += link_prl
CONFIG += app_bundle

INCLUDEPATH += $$PWD/../../Impl

INCLUDEPATH += $(ACFDIR)/$$AUXINCLUDEDIR
INCLUDEPATH += $(ACFSLNDIR)/$$AUXINCLUDEDIR


DESTDIR = ../../../Bin/$$COMPILER_DIR

HEADERS += ../*.h
SOURCES += ../*.cpp
