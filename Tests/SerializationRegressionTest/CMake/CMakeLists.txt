project(SerializationRegressionTest)

set(ARXC_CONFIG "${CMAKE_CURRENT_LIST_DIR}/../../../Config/AcfTestConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)
include(${ACFDIR}/Config/CMake/CustomBuild.cmake)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

target_link_libraries(${PROJECT_NAME} ipackage icomp i2d i3d icmm imath iser istd iprm ifile iimg ibase imod itest)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core)

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${PROJECT_NAME}.exe\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
endif()
