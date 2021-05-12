cmake_minimum_required(VERSION 3.1.0)

set(QT_DEFAULT_MAJOR_VERSION 5)

message("CMAKE_VERSION " ${CMAKE_VERSION})

if(DEFINED ENV{QTDIR})
	set(CMAKE_PREFIX_PATH $ENV{QTDIR})
	message("set CMAKE_PREFIX_PATH " ${CMAKE_PREFIX_PATH})
	message("Qt5Core_DIR " ${Qt5Core_DIR})
endif()

#Acf
if(NOT DEFINED ACFDIR)
	set(ACFDIR "$ENV{ACFDIR}")
endif()
if(NOT DEFINED ACFCONFIGDIR)
	set(ACFCONFIGDIR "$ENV{ACFCONFIGDIR}")
endif()

include("${ACFDIR}/Config/CMake/Macro.cmake")

get_target_name(TARGETNAME)

include_directories("${ACFDIR}/AuxInclude/Qt${QT_DEFAULT_MAJOR_VERSION}_${TARGETNAME}")
include_directories("${ACFDIR}/Include")
include_directories("${ACFDIR}/Impl")

