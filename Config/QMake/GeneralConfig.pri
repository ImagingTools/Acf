#General ACF settings

CONFIG += stl
CONFIG -= exceptions
CONFIG += silent
CONFIG += rtti

COMPILER_NAME = QMake
win32-msvc*{
	COMPILER_NAME = VC

	win32-msvc2005{
		COMPILER_NAME = VC8
	}
	win32-msvc2008{
		COMPILER_NAME = VC9
	}
	win32-msvc2010{
		COMPILER_NAME = VC10
	}
}

QMAKE_CXXFLAGS_WARN_ON = -Wall

CONFIG(debug, debug|release){
	COMPILER_DIR = Debug$$COMPILER_NAME
}
CONFIG(release, debug|release){
	COMPILER_DIR = Release$$COMPILER_NAME
	CONFIG += sse2
}

!win32-msvc*{
	QMAKE_LFLAGS -= -mthreads
	#QMAKE_CXXFLAGS += -fvisibility=hidden
	QMAKE_CXXFLAGS += -fno-threadsafe-statics
	QMAKE_CXXFLAGS += -fvisibility-ms-compat
}

INCLUDEPATH += $$PWD/../../Include
LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR

HEADERS += ../*.h
SOURCES += ../*.cpp
