project(QtViewPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

target_link_libraries(${PROJECT_NAME} ${ACF_PACKAGE_LINK_SCOPE}
	ibase
	icomp
	iview
	iqt2d
)

