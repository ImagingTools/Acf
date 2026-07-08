project(BasePck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	idoc
	i2d
	ilog
	ibase
	icomp
	imath
)

