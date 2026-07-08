project(QtViewPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	ibase
	icomp
	iview
	iqt2d
)

