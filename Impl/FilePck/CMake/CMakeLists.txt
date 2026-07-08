project(FilePck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	ifile
	icomp
)

