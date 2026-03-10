TARGET = istd

include(../../../Config/QMake/StaticConfig.pri)

win32 {
	LIBS += -lMpr
}

