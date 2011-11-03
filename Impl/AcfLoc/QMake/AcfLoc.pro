include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/CustomBuild.pri)

TARGET = AcfLoc


HEADERS =
SOURCES =
COPYQMTOGENERATE += $(QTDIR)/translations/qt_de.qm
COPYQMTOGENERATE += $(QTDIR)/translations/qt_ru.qm
COPYQMTOGENERATE += $(QTDIR)/translations/qt_pl.qm
TRANSLATIONS += ../*.ts
RESOURCES += ../*.qrc
