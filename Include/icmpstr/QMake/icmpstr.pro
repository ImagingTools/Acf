include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = icmpstr

RESOURCES += ../*.qrc

LIBS += -licomp
LIBS += -liqtgui
