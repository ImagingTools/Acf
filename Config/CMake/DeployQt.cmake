#[[==========================================================================
Deploys Qt runtime dependencies for a target on supported desktop platforms.

On Windows, uses windeployqt. On macOS, configures an application bundle and
uses macdeployqt. Other platforms are a no-op.

DEPLOY_QT_QML_DIRS (Project-level optional) List of directories passed to the
platform deployment tool for QML import discovery. Set this variable before
calling deploy_qt() when QML imports used only via string-based lookups must
be deployed.

\param TARGET  (required) CMake target to deploy Qt dependencies for.
\param OPTIONS (optional) Additional options passed directly to the platform
                 deployment tool.

Usage:
	deploy_qt(TARGET ${PROJECT_NAME})
==========================================================================]]

if(WIN32)
	include("${CMAKE_CURRENT_LIST_DIR}/WinDeployQt.cmake")
elseif(APPLE)
	include("${CMAKE_CURRENT_LIST_DIR}/MacDeployQt.cmake")
endif()

function(deploy_qt)
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
		message(FATAL_ERROR "deploy_qt: TARGET is required")
	endif()

	if(WIN32)
		win_deploy_qt(TARGET ${ARG_TARGET} OPTIONS ${ARG_OPTIONS})
	elseif(APPLE)
		mac_deploy_qt(TARGET ${ARG_TARGET} OPTIONS ${ARG_OPTIONS})
	endif()
endfunction()
