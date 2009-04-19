TARGET = WinPck
TEMPLATE = lib

CONFIG += dll

CONFIG(debug, debug|release) {
	DESTDIR = $(ACFDIR)\Bin\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = $(ACFDIR)\Bin\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ 

