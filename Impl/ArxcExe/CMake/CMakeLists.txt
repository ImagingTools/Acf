include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

project(Arxc LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

if(WIN32)
	windeployqt(${PROJECT_NAME})
endif()
