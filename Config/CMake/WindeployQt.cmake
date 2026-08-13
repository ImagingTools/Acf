find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable

function(windeploy target listOptions listFiles)
	get_target_property(qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
	get_filename_component(qt_bin_dir "${qmake_executable}" DIRECTORY)
	set(windeployqt_executable ${qt_bin_dir}/windeployqt.exe)

	message("windeployqt_executable ${windeployqt_executable} ${listOptions} ${listFiles}")

	set(deployArgs ${listOptions} ${listFiles})

	# All targets deploy into the same output directory, windeployqt is not safe to run
	# concurrently on it, therefore the calls are serialized by RunWindeployQt.cmake.
	add_custom_command(
		TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		"-DTOOL=${windeployqt_executable}"
		"-DARGS=${deployArgs}"
		"-DLOCK_FILE=${CMAKE_BINARY_DIR}/windeployqt.lock"
		-P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/RunWindeployQt.cmake"
		COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
		VERBATIM
		)
endfunction()

