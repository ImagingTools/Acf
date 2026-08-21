project(iimgTest)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test Gui)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	iimg
	itest
	Qt${QT_VERSION_MAJOR}::Test
	Qt${QT_VERSION_MAJOR}::Gui
)


windeploy(TARGET ${PROJECT_NAME})
