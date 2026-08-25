# General base configuration should be included in all ACF-based projects

message(VERBOSE "PROJECT_NAME " ${PROJECT_NAME})
message(VERBOSE "CMAKE_CURRENT_LIST_DIR " ${CMAKE_CURRENT_LIST_DIR})
message(VERBOSE "PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
message(VERBOSE "BINARY_DIR " ${BINARY_DIR})

set(COMPILER_NAME "CLang")

if(${MSVC})
	set(COMPILER_NAME "VC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127 /wd4250 /wd4347 /wd4355 /wd4365 /wd4481 /wd4505 /wd4510 /wd4511 /wd4512 /wd4548 /wd4571 /wd4619 /wd4625 /wd4626 /wd4640 /wd4702 /wd4710 /wd4820 /wd4826 /we4701")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /fp:fast")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

	if(${MSVC_TOOLSET_VERSION} STREQUAL 90)
		string(REPLACE "/Gd" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		string(REPLACE "/GD" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		string(REPLACE "-Gd" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		string(REPLACE "-GD" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

	elseif(${MSVC_TOOLSET_VERSION} STREQUAL 100 OR ${MSVC_TOOLSET_VERSION} STREQUAL 110)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")

	elseif(${MSVC_TOOLSET_VERSION} STREQUAL 120)
		add_compile_options(-std=c++11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /Qpar /Gy /Gw /FS")

	elseif(${MSVC_TOOLSET_VERSION} GREATER_EQUAL 140 AND ${MSVC_TOOLSET_VERSION} LESS_EQUAL 142)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qpar /Gy /Gw /FS /Zc:threadSafeInit-")
		set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT} /Ot /Oi /Ob2 /GS-")
		if(${MSVC_TOOLSET_VERSION} STREQUAL 140 AND ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} STREQUAL x64)
			set(CMAKE_CXX_LINK_LIBRARY_FLAG "${CMAKE_CXX_LINK_LIBRARY_FLAG} /MACHINE:X64")
		endif()

	elseif(${MSVC_TOOLSET_VERSION} GREATER_EQUAL 143)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gy /FS /Zc:threadSafeInit-")
		set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT} /Ot /Oi /Ob2 /GS-")
	endif()

	# Avoid repeating this message for every ACF subproject that includes this file
	get_property(ACF_TOOLSET_MESSAGE_PRINTED GLOBAL PROPERTY ACF_TOOLSET_MESSAGE_PRINTED)
	if(NOT ACF_TOOLSET_MESSAGE_PRINTED)
		message(STATUS "ACF: MSVC_TOOLSET_VERSION=${MSVC_TOOLSET_VERSION}, COMPILER_NAME=${COMPILER_NAME}")
		set_property(GLOBAL PROPERTY ACF_TOOLSET_MESSAGE_PRINTED TRUE)
	endif()
endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(AUXINCLUDEDIR "AuxInclude/${TARGETNAME}/GeneratedFiles")
set(AUXINCLUDEPATH "${PROJECT_SOURCE_DIR}/../../../${AUXINCLUDEDIR}")

include_directories("${PROJECT_SOURCE_DIR}/../../")
include_directories("${INCLUDE_DIR}")
include_directories("${IMPL_DIR}")


function(acf_create_moc outfiles)
	if(QT_VERSION_MAJOR EQUAL 5)
		qt5_wrap_cpp("${outfiles}" ${ARGN} )
	elseif(QT_VERSION_MAJOR EQUAL 6)
		qt6_wrap_cpp("${outfiles}" ${ARGN} )
	endif()
	set("${outfiles}" "${${outfiles}}" PARENT_SCOPE)
endfunction()

function(acf_wrap_resources outfiles)
	if(QT_VERSION_MAJOR EQUAL 5)
		qt5_add_resources("${outfiles}" ${ARGN})
	elseif(QT_VERSION_MAJOR EQUAL 6)
		qt6_add_resources("${outfiles}" ${ARGN})
	endif()
	set("${outfiles}" "${${outfiles}}" PARENT_SCOPE)
endfunction()

function(acf_wrap_ui outfiles)
	if(QT_VERSION_MAJOR EQUAL 5)
		qt5_wrap_ui("${outfiles}" ${ARGN})
	elseif(QT_VERSION_MAJOR EQUAL 6)
		qt6_wrap_ui("${outfiles}" ${ARGN})
	endif()
	set("${outfiles}" "${${outfiles}}" PARENT_SCOPE)
endfunction()

# ---------------------------------------------------------------------------
# Configurable link-scope variables.
#
# These are normally defined by the top-level CMakeLists.txt via
# validated CACHE entries created by the environment setup.
# The guards below provide safe defaults when GeneralConfig.cmake is included
# without a prior call (e.g. by downstream repos that include individual
# library CMakeLists directly).
# ---------------------------------------------------------------------------
if(NOT DEFINED ACF_QT_MODULE_LINK_SCOPE)
	set(ACF_QT_MODULE_LINK_SCOPE "")
endif()
if(NOT DEFINED ACF_LIBRARY_LINK_SCOPE)
	set(ACF_LIBRARY_LINK_SCOPE "")
endif()
if(NOT DEFINED ACF_PACKAGE_LINK_SCOPE)
	set(ACF_PACKAGE_LINK_SCOPE "")
endif()
if(NOT DEFINED ACF_APPLICATION_LINK_SCOPE)
	set(ACF_APPLICATION_LINK_SCOPE "")
endif()

function(acf_use_qt_base_modules)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Core)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Xml)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Network)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Concurrent)
endfunction()

function(acf_use_qt_graphics_modules)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Widgets)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Gui)
	target_link_libraries(${PROJECT_NAME} ${ACF_QT_MODULE_LINK_SCOPE} Qt${QT_VERSION_MAJOR}::Svg)
endfunction()


# ---------------------------------------------------------------------------
# Modern, target-based packaging support
#
# The helpers below turn every ACF static library into a first-class,
# relocatable target that carries its own usage requirements (include paths
# and link dependencies) and can be consumed through find_package(Acf).
#
# They are additive: the legacy global include_directories()/link_directories()
# calls are kept as a backward-compatibility shim so the existing in-tree build
# keeps working unchanged while downstream modules migrate to find_package.
# ---------------------------------------------------------------------------

# Name of the export set that aggregates all installable library targets for the
# current repository.  Each repo sets ACF_PACKAGE_NAME before including this
# file so that its targets are exported under the correct package/namespace
# (e.g. "Acf", "AcfSln", "IAcf", "ImtCore").  Defaults to "Acf" for backward
# compatibility with the original Acf repository.
if(NOT DEFINED ACF_PACKAGE_NAME)
	set(ACF_PACKAGE_NAME "Acf")
endif()

if(NOT DEFINED ACF_EXPORT_SET)
	set(ACF_EXPORT_SET "${ACF_PACKAGE_NAME}Targets")
endif()

# Register a library target so that:
#  * it exposes the source roots as transitive include directories,
#  * it is available under the namespaced "${ACF_PACKAGE_NAME}::<target>" alias,
#  * it becomes part of the exported/installable package.
#
# This is called from StaticConfig.cmake right after the library is created.
function(acf_register_library target)
	# Transitive include directories: consumers (in-tree via the alias or
	# downstream via find_package) inherit the header search paths without
	# having to know the internal directory layout.
	target_include_directories(${target}
		PUBLIC
			$<BUILD_INTERFACE:${INCLUDE_DIR}>
			$<BUILD_INTERFACE:${IMPL_DIR}>
			$<INSTALL_INTERFACE:include>
	)

	# Namespaced alias so the same "<Package>::<lib>" spelling works both
	# in-tree and for downstream consumers of the exported package.
	if(NOT TARGET ${ACF_PACKAGE_NAME}::${target})
		add_library(${ACF_PACKAGE_NAME}::${target} ALIAS ${target})
	endif()

	# Register the target for installation and export. The actual
	# install(EXPORT ...) / package-config generation happens once, centrally,
	# in the repo's PackageExport cmake module.
	install(TARGETS ${target}
		EXPORT ${ACF_EXPORT_SET}
		ARCHIVE DESTINATION "lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}"
		LIBRARY DESTINATION "lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}"
		RUNTIME DESTINATION "bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}"
		INCLUDES DESTINATION "include")

	# Install the public headers preserving the "<lib>/<header>.h" layout that
	# the #include directives rely on. PROJECT_SOURCE_DIR points at the library's
	# CMake/ directory, so its parent is the library source root.
	install(DIRECTORY "${PROJECT_SOURCE_DIR}/../"
		DESTINATION "include/${target}"
		FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hpp"
			PATTERN "*.hxx"
			PATTERN "CMake" EXCLUDE
			PATTERN "Test" EXCLUDE)
endfunction()

