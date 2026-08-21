project(FileComponentTest LANGUAGES CXX)


set(ARXC_CONFIG "${CMAKE_CURRENT_LIST_DIR}/../../../Config/AcfTestConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)
include(${ACFDIR}/Config/CMake/CustomBuild.cmake)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	ifile
	itest
	Qt${QT_VERSION_MAJOR}::Test
)


windeploy(TARGET ${PROJECT_NAME})
