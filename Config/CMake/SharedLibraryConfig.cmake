#Standard settings for an ACF shared library project

include(${ACFDIR}/Config/CMake/GeneralConfig.cmake)

include_directories(${PROJECT_SOURCE_DIR}/../../../../)

file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")

file(GLOB PROJECT_SRC
        ${HEADER_FILES}
        ${SOURCES_FILES}
)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})

acf_create_moc(MOC_SOURCES "${HEADER_FILES}")
acf_wrap_resources(RESOURCES_FILES "${QRC_FILES}")
acf_wrap_ui(UI_SOURCES "${UI_FILES}")

target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${UI_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FILE_AUX})

#qt5_use_modules(${PROJECT_NAME} QuickWidgets)
#find_package(qtlib Qt${QT_DEFAULT_MAJOR_VERSION}::Core REQUIRED)
#target_link_libraries(Qt${QT_DEFAULT_MAJOR_VERSION}::QuickWidgets)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(outbindir "${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}${TARGETNAME}/Plugins")

if(WIN32)
	#set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
	#set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

#INCLUDEPATH += $$OUT_PWD/../../../Include
#INCLUDEPATH += $$_PRO_FILE_PWD_/../../../Include
#INCLUDEPATH += $$PWD/../../Impl
#
#TEMPLATE = lib
#
#DESTDIR = $$OUT_PWD/../../../Bin/$$COMPILER_DIR
#
#CONFIG += link_prl
#CONFIG += shared
#
#!win32-msvc*{
#	QMAKE_CFLAGS += -fPIC
#}
#
#HEADERS += $$files($$_PRO_FILE_PWD_/../*.h, true)
#SOURCES += $$files($$_PRO_FILE_PWD_/../*.cpp, true)
#
#isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
#	include(MsvcEnableDebugInfo.pri)
#}
