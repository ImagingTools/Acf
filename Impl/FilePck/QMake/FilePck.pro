TARGET = FilePck

include(../../../Config/QMake/ComponentConfig.pri)

QT += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../../Config/QMake/AcfStd.pri)

LIBS += -L$(QTDIR)/lib -lQt5Concurrent

