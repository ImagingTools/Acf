TARGET = imath
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ 

HEADERS += ../CComplexDoubleManip.h
HEADERS += ../CDouble.h
HEADERS += ../CDoubleManip.h
HEADERS += ../CFixedPointManip.h
HEADERS += ../imath.h
HEADERS += ../TFastVector.h
HEADERS += ../TFulcrumGridFunctionBase.h
HEADERS += ../TIMathFunction.h
HEADERS += ../TIValueManip.h
HEADERS += ../TMathVectorWrap.h
HEADERS += ../TSplineGridFunctionBase.h
HEADERS += ../TVarMatrix.h
HEADERS += ../TVarVector.h
HEADERS += ../TVector.h
SOURCES += ../CComplexDoubleManip.cpp
SOURCES += ../CDoubleManip.cpp
SOURCES += ../CFixedPointManip.cpp
