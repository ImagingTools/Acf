project(BitmapPck LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
	iimg
	icomp
)

acf_use_qt_graphics_modules()

