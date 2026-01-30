project(istd LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} Mpr)
endif()

