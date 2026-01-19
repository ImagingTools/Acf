project(iimgprint LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

target_link_libraries(${PROJECT_NAME} icmm)
target_link_libraries(${PROJECT_NAME} iimg)
target_link_libraries(${PROJECT_NAME} imath)

