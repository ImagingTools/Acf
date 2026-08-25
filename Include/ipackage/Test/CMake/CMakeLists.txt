project(ipackageTest)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/DeployQt.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	ipackage
	itest
	Qt${QT_VERSION_MAJOR}::Test
)


deploy_qt(TARGET ${PROJECT_NAME})
