project(ifile LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

target_link_libraries(${PROJECT_NAME} PUBLIC
	Qt${QT_VERSION_MAJOR}::Concurrent
)

