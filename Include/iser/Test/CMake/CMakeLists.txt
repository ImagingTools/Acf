project(iserTest)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	iser

	idoc
	itest
	Qt${QT_VERSION_MAJOR}::Test
)

windeploy(TARGET ${PROJECT_NAME})
