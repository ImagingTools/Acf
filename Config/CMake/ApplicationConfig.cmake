include(${CMAKE_CURRENT_LIST_DIR}/GeneralConfig.cmake)

message("PROJECT_SOURCE_DIR "  ${PROJECT_SOURCE_DIR})
message("CMAKE_PROJECT_HOMEPAGE_URL "  ${CMAKE_PROJECT_HOMEPAGE_URL})

file(GLOB UI_FILES "${PROJECT_SOURCE_DIR}/../*.ui")
file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")
file(GLOB QRC_FILES "${PROJECT_SOURCE_DIR}/../*.qrc")

file(GLOB PROJECT_SRC
    ${HEADER_FILES}
    ${SOURCES_FILES}
	${RC_FILE}
)

message("SOURCES_FILE_AUX " "${SOURCES_FILE_AUX}")

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

acf_create_moc(MOC_SOURCES "${HEADER_FILES}")
acf_wrap_resources(RESOURCES_FILES "${QRC_FILES}")
acf_wrap_ui(UI_SOURCES "${UI_FILES}")

target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${UI_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FILE_AUX})
target_sources(${PROJECT_NAME} PRIVATE ${RC_FILE})


qt5_use_modules(${PROJECT_NAME} Core Widgets Gui Xml Network XmlPatterns Svg Concurrent)

set(outbindir "${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}${TARGETNAME}")
set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${outbindir})

#set(EXECUTABLE_OUTPUT_PATH "${ACFDIRBUILD}")
