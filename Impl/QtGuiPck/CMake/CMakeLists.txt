project(QtGuiPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	iqtgui
	iqtprm
	iqtdoc
	iloggui
	icomp
	ibase
)

