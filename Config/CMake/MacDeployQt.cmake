#[[==========================================================================
Configures and deploys a Qt application as a macOS bundle.

Creates a macOS application bundle and deploys the required Qt libraries using macdeployqt. 
If present, the conventional ../Mac/Info.plist and ../Mac/<target>.icns files are added to the bundle.

If those files are absent, CMake's default Info.plist is used and no custom
application icon is added.

DEPLOY_QT_QML_DIRS (Project-level optional)  list of directories passed
									to macdeployqt as -qmldir=<dir> for each entry. 
									Set this variable before calling mac_deploy_qt() when 
									QML imports used only via string-based lookups must be deployed.

\param TARGET			(required) Target to configure and deploy.
\param OPTIONS			(optional) Additional options passed directly to macdeployqt.

Usage:
	set(DEPLOY_QT_QML_DIRS ${ARG_QMLDIRS})

	mac_deploy_qt(
		TARGET ${PROJECT_NAME}
		OPTIONS "-no-translations"
	)

Minimal usage:
	mac_deploy_qt(TARGET ${PROJECT_NAME})
==========================================================================]]
function(mac_deploy_qt)
	if (NOT APPLE)
		return()
	endif()

	set(oneValueArgs TARGET)
	set(multiValueArgs OPTIONS)

	cmake_parse_arguments(
		ARG
		""
		"${oneValueArgs}"
		"${multiValueArgs}"
		${ARGN}
	)

	if(NOT ARG_TARGET)
		message(FATAL_ERROR "mac_deploy_qt: TARGET is required")
	endif()

	get_target_property(QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
	get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY )
	set(DEPLOY_QT_EXECUTABLE "${QT_BIN_DIR}/macdeployqt")
	if(NOT EXISTS "${DEPLOY_QT_EXECUTABLE}")
		message(FATAL_ERROR "Unable to find macdeployqt: ${DEPLOY_QT_EXECUTABLE}")
	endif()

	set_property(
		TARGET ${ARG_TARGET}
		PROPERTY MACOSX_BUNDLE TRUE
	)

	set(INFO_PLIST "${PROJECT_SOURCE_DIR}/../Mac/Info.plist")
	if(EXISTS "${INFO_PLIST}")
		set_target_properties(${ARG_TARGET} PROPERTIES
			MACOSX_BUNDLE_INFO_PLIST "${INFO_PLIST}"
		)
	endif()

	set(ICON "${PROJECT_SOURCE_DIR}/../Mac/${ARG_TARGET}.icns")
	if(EXISTS "${ICON}")
		get_filename_component(ICON_FILE_NAME "${ICON}" NAME)
		set_target_properties(${ARG_TARGET} PROPERTIES MACOSX_BUNDLE_ICON_FILE "${ICON_FILE_NAME}")
		target_sources(${ARG_TARGET} PRIVATE "${ICON}")
		set_source_files_properties("${ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources" )
	endif()

	set(DEPLOY_OPTIONS ${ARG_OPTIONS})
	foreach(QML_DIR IN LISTS DEPLOY_QT_QML_DIRS)
		list(APPEND DEPLOY_OPTIONS "-qmldir=${QML_DIR}")
	endforeach()

	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		list(APPEND DEPLOY_OPTIONS "-no-strip")
	endif()

	add_custom_command(
		TARGET ${ARG_TARGET}
		POST_BUILD
		COMMAND
			"${DEPLOY_QT_EXECUTABLE}"
			"$<TARGET_BUNDLE_DIR:${ARG_TARGET}>"
			${DEPLOY_OPTIONS}
		COMMENT
			"Deploying Qt libraries using ${DEPLOY_QT_EXECUTABLE}"
		VERBATIM
	)
endfunction()