cmake_minimum_required(VERSION 3.26)

if(NOT DEFINED ACFDIR)
	set(ACFDIR "$ENV{ACFDIR}")
endif()

# AcfEnv.cmake includes ProjectRoot.cmake and calls get_target_name(TARGETNAME)
# which defines the TARGETNAME variable used later in this file
include(${ACFDIR}/Config/CMake/AcfEnv.cmake)

project(IqtComponentTest LANGUAGES CXX)


set(ARXC_CONFIG "${CMAKE_CURRENT_LIST_DIR}/../../../Config/AcfTestConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)
include(${ACFDIR}/Config/CMake/CustomBuild.cmake)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

target_link_libraries(${PROJECT_NAME}
	iqt
	ifile
	iprm
	itest
	Qt${QT_VERSION_MAJOR}::Test
	Qt${QT_VERSION_MAJOR}::Core
)


if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${PROJECT_NAME}.exe\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
endif()
