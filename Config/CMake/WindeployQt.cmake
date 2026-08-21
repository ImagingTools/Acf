#[[==========================================================================
Deploys Qt runtime dependencies for a Windows target using windeployqt.

No-op on non-Windows platforms (WIN32 check returns early).

Serializes concurrent windeployqt invocations across targets
via RunWindeployQt.cmake, since windeployqt is not safe to run concurrently
against the same output directory.

\param TARGET			(required) The CMake target to deploy Qt dependencies for.
									A POST_BUILD step is attached to this target.
\param TARGET_FILE_NAME	 (optional) Basename of the deployed file, if it differs from TARGET.
									Defaults to TARGET.
\param EXT				(optional) File extension of the deployed binary,
									without the leading dot (e.g. "exe", "arp").
									If omitted, deduced from the target's TYPE
									property:
										EXECUTABLE     -> exe
										SHARED_LIBRARY -> arp
									Any other target type requires EXT to be passed
									explicitly; otherwise a FATAL_ERROR is raised.
\param QML_DIRS			(optional) List of directories passed to
									windeployqt as --qmldir=<dir> for each entry, so
									QML imports used only via string-based lookups
									(i.e. not visible to static import scanning) are
									still deployed.
\param OPTIONS			(optional) Additional raw options forwarded to
									windeployqt as-is, ahead of the deployed file
									path.
Usage:
	windeploy(TARGET ${PROJECT_NAME})

	windeploy(
		TARGET ${PROJECT_NAME}
		QML_DIRS ${ARG_QMLDIRS}
	)

	windeploy(
		TARGET ${PROJECT_NAME}
		TARGET_FILE_NAME "${PROJECT_NAME}Core"
		EXT "dll"
		OPTIONS "-no-translations"
	)
==========================================================================]]

function(windeploy)
	if(NOT WIN32)
		return()
	endif()

	set(oneValueArgs
		TARGET
		TARGET_FILE_NAME
		EXT
	)
	set(multiValueArgs
		QML_DIRS
		OPTIONS
	)
	cmake_parse_arguments(
		ARG
		""
		"${oneValueArgs}"
		"${multiValueArgs}"
		${ARGN}
	)

	if(NOT ARG_TARGET)
		message(FATAL_ERROR "windeploy: TARGET is required")
	endif()

	if(NOT ARG_TARGET_FILE_NAME)
		set(ARG_TARGET_FILE_NAME "${ARG_TARGET}")
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

	# Use the caller-supplied EXT when given; otherwise deduce it from the
	# target type. Unknown types must pass EXT explicitly.
	if(ARG_EXT)
		set(EXT "${ARG_EXT}")
	else()
		get_target_property(TARGET_TYPE
			${ARG_TARGET}
			TYPE
		)
		if(TARGET_TYPE STREQUAL "EXECUTABLE")
			set(EXT "exe")
		elseif(TARGET_TYPE STREQUAL "SHARED_LIBRARY")
			set(EXT "arp")
		else()
			message(FATAL_ERROR "windeploy: cannot deduce EXT for target '${ARG_TARGET}' of type ${TARGET_TYPE}; pass EXT explicitly")
		endif()
	endif()

	set(DEPLOY_OPTIONS ${ARG_OPTIONS})
	foreach(QML_DIR IN LISTS ARG_QML_DIRS)
		list(APPEND DEPLOY_OPTIONS "--qmldir=${QML_DIR}")
	endforeach()

	get_target_property(OUTPUT_DIRECTORY
		${ARG_TARGET}
		RUNTIME_OUTPUT_DIRECTORY
	)
	set(DEPLOY_FILE "${OUTPUT_DIRECTORY}/${ARG_TARGET_FILE_NAME}.${EXT}")
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