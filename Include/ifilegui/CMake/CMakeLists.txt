project(ifilegui LANGUAGES CXX)


include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

if(QT_VERSION_MAJOR EQUAL 6)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		Qt${QT_VERSION_MAJOR}::SvgWidgets
		Qt${QT_VERSION_MAJOR}::Concurrent
	)
endif()

