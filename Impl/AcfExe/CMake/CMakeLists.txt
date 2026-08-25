project(Acf LANGUAGES CXX)


set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/DeployQt.cmake)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	ipackage
	Qt${QT_VERSION_MAJOR}::Widgets
)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE} user32)
endif()

deploy_qt(TARGET ${PROJECT_NAME})

