TARGET = UuidComponentTest

include(../../../Config/QMake/Application.pri)

# Link with required libraries
LIBS += -litest -libase -liprm -liser -listd

# Qt modules
QT += testlib

# Test configuration
ARXC_CONFIG = ../../../Config/AcfTestConfig.awc
ARXC_FILES = ../$${TARGET}.acc
