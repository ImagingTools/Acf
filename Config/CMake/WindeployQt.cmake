find_package(Qt5Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

message("_qt_bin_dir " ${_qt_bin_dir}/windeployqt.exe)

function(windeployqt target)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs

    add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${Qt5_DIR}/../../../bin/windeployqt.exe"
#COMMAND "$ENV{QTDIR}/bin/windeployqt.exe"
#COMMAND "${_qt_bin_dir}/windeployqt.exe"
#ARGS --verbose 1 ${AUX_INCLUDE_DIR}/../../../Bin/${TARGETNAME}
                --verbose 1
#                --release
#                --no-svg
#                --no-angle
#                --no-opengl
#                --no-opengl-sw
#                --no-compiler-runtime
#                --no-system-d3d-compiler
#                \"$<TARGET_FILE:${target}>\"
                \"${AUX_INCLUDE_DIR}/../../../Bin/${TARGETNAME}\"

		COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )

endfunction()
