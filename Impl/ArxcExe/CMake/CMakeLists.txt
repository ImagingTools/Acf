project(Arxc LANGUAGES CXX)


set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

include(${ACFDIR}/Config/CMake/WindeployQt.cmake)
include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	QtPck
	BasePck
	FilePck
	PackagePck
	icomp
	istd
	ipackage
)

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
	unset(listOptions)
endif()

mac_deploy_qt(
	REMOVE_DEBUGSYM
	TARGET ${PROJECT_NAME}
	TARGET_FILE_NAME ${PROJECT_NAME}
)

