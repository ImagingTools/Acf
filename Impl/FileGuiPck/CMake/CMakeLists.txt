project(FileGuiPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	ifilegui
)

acf_use_qt_graphics_modules()

if(WIN32)
	set(files \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${PROJECT_NAME}.arp\")
	windeploy(${PROJECT_NAME} "" "${files}")
endif()

