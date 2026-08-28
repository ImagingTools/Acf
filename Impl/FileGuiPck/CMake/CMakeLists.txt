project(FileGuiPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/DeployQt.cmake)

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	ifilegui
)

deploy_qt(TARGET ${PROJECT_NAME})

