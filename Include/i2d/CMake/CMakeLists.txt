project(i2d LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Gui)

