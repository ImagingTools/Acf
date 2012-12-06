include(../../../Config/QMake/StaticConfig.pri)

TARGET = ibase

mac{
	LIBS += -framework Carbon
}
