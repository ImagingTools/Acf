# Standard settings for an ACF test

include($$(ACFCONFIGDIR)/QMake/GeneralConfig.pri)

TEMPLATE = app

QT = widgets testlib
CONFIG += testcase

LIBS += -L$$(ACFDIR)/Lib/$$COMPILER_DIR -liser -listd -litest

DESTDIR = $$OUT_PWD/../../../../Bin/$$COMPILER_DIR/Tests
SOURCES += $$_PRO_FILE_PWD_/../*.cpp

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include($$(ACFCONFIGDIR)/QMake/MsvcEnableDebugInfo.pri)
}

win32:QMAKE_POST_LINK = "$$(QTDIR)\\bin\\windeployqt $$system_path($${DESTDIR})\\$${TARGET}.exe>NUL $$escape_expand(\n\t)"