project(TestComposedGui LANGUAGES CXX)

if(WIN32)
	set(CMAKE_WIN32_EXECUTABLE "on")
endif()

set(ARXC_CONFIG "${PROJECT_SOURCE_DIR}/../../../Config/AcfConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

include(${ACFDIR}/Config/CMake/CustomBuild.cmake)
include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	AcfLoc
	icomp
	iqtgui
	iqtprm
	iwidgets
)

windeploy(TARGET ${PROJECT_NAME})

