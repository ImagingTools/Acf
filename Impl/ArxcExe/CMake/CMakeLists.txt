project(Arxc LANGUAGES CXX)


set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

include(${ACFDIR}/Config/CMake/DeployQt.cmake)
include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)

target_link_libraries(${PROJECT_NAME} ${ACF_APPLICATION_LINK_SCOPE}
	ipackage
)

deploy_qt(TARGET ${PROJECT_NAME})

