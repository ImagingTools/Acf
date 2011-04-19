include(../../../Config/QMake/Include.config)

TARGET = istd

HEADERS += ../Generated/AcfVersion.h

SVNTR_REPOSITORY = ../../..
SVNTR_DESTINATION = ../Generated
SVNTR_FILES += ../AcfVersion.h.xtrsvn

include(../../../Config/QMake/CustomBuild.pri)

