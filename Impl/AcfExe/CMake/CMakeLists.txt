project(Acf LANGUAGES CXX)

set(ARX_DISABLE_GENERATE_DEPENDENCIES_LIST ON)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

qt5_use_modules(${PROJECT_NAME} Xml)
