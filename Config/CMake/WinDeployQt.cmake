#[[==========================================================================
Deploys Qt runtime dependencies for a Windows target using windeployqt.

No-op on non-Windows platforms (WIN32 check returns early).

Serializes concurrent windeployqt invocations across targets
via RunWindeployQt.cmake, since windeployqt is not safe to run concurrently
against the same output directory.

DEPLOY_QT_QML_DIRS (Project-level optional) 	Project-level list of directories passed to
												windeployqt as --qmldir=<dir> for each entry. Set this
																		variable before calling win_deploy_qt() when QML imports used
												only via string-based lookups must be deployed.

\param TARGET			(required) 		The CMake target to deploy Qt dependencies for.
										A POST_BUILD step is attached to this target.
\param OPTIONS			(optional) 		Additional raw options forwarded to
										windeployqt as-is, ahead of the deployed file
										path.
Usage:
	set(DEPLOY_QT_QML_DIRS ${ARG_QMLDIRS})

	win_deploy_qt(
		TARGET ${PROJECT_NAME}
		OPTIONS "-no-translations"
	)

Minimal usage:
	win_deploy_qt(TARGET ${PROJECT_NAME})
==========================================================================]]

function(win_deploy_qt)
	if(NOT WIN32)
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
		message(FATAL_ERROR "win_deploy_qt: TARGET is required")
	endif()

	get_target_property(QMAKE_EXECUTABLE
		Qt${QT_VERSION_MAJOR}::qmake
		IMPORTED_LOCATION
	)

	get_filename_component(QT_BIN_DIR
		"${QMAKE_EXECUTABLE}"
		DIRECTORY
	)

	set(WINDEPLOYQT_TARGET "${QT_BIN_DIR}/windeployqt.exe")
	if(NOT EXISTS "${WINDEPLOYQT_TARGET}")
		message(FATAL_ERROR
			"Unable to find windeployqt: ${WINDEPLOYQT_TARGET}"
		)
	endif()

	set(DEPLOY_OPTIONS ${ARG_OPTIONS})
	foreach(QML_DIR IN LISTS DEPLOY_QT_QML_DIRS)
		list(APPEND DEPLOY_OPTIONS "--qmldir=${QML_DIR}")
	endforeach()

	set(DEPLOY_FILE "$<TARGET_FILE:${ARG_TARGET}>")
	set(DEPLOY_ARGS ${DEPLOY_OPTIONS} ${DEPLOY_FILE})

	message("windeployqt: ${WINDEPLOYQT_TARGET} ${DEPLOY_OPTIONS} ${DEPLOY_FILE}")

	# All targets deploy into the same output directory, windeployqt is not safe to run
	# concurrently on it, therefore the calls are serialized by RunWindeployQt.cmake.
	add_custom_command(
		TARGET ${ARG_TARGET}
		POST_BUILD
		COMMAND
			${CMAKE_COMMAND}
			"-DTOOL=${WINDEPLOYQT_TARGET}"
			"-DARGS=${DEPLOY_ARGS}"
			"-DLOCK_FILE=${CMAKE_BINARY_DIR}/windeployqt.lock"
			-P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/RunWindeployQt.cmake"
		COMMENT
			"Deploying Qt libraries using windeployqt for compilation target '${ARG_TARGET}' ..."
		VERBATIM
	)
endfunction()