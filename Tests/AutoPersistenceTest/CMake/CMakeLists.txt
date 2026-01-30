project(AutoPersistenceTest LANGUAGES CXX)


set(ARXC_CONFIG "${CMAKE_CURRENT_LIST_DIR}/../../../Config/AcfTestConfig.awc")
set(ARXC_FILES 
    "${PROJECT_SOURCE_DIR}/../AutoPersistenceTestRestoreStoreEnd.acc"
    "${PROJECT_SOURCE_DIR}/../AutoPersistenceTestStoreOnChange.acc"
    "${PROJECT_SOURCE_DIR}/../AutoPersistenceTestStoreOnBegin.acc"
    "${PROJECT_SOURCE_DIR}/../AutoPersistenceTestStoreInterval.acc"
    "${PROJECT_SOURCE_DIR}/../AutoPersistenceTestAutoReload.acc"
    "${PROJECT_SOURCE_DIR}/../AutoPersistenceTestLocking.acc"
)
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)
include(${ACFDIR}/Config/CMake/CustomBuild.cmake)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

target_link_libraries(${PROJECT_NAME}
	iprm
	ifile
	iser
	itest
	Qt${QT_VERSION_MAJOR}::Test
	Qt${QT_VERSION_MAJOR}::Core
)


if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${PROJECT_NAME}.exe\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
endif()
