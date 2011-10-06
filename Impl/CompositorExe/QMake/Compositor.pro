include($(ACFDIR)/Config/QMake/ApplicationConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)
include($(ACFDIR)/Impl/AcfQt/QMake/AcfQt.pri)

TARGET = Compositor

RESOURCES += ../*.qrc

LIBS += -licmpstr

QT += xml

ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

include($(ACFDIR)/Config/QMake/CustomBuild.pri)

