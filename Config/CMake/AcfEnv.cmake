cmake_minimum_required(VERSION 3.26)


# Acf
if(NOT DEFINED ACFDIR)
	set(ACFDIR "$ENV{ACFDIR}")
endif()

if(NOT DEFINED QTDIR)
	file(TO_CMAKE_PATH "$ENV{QTDIR}" QTDIR)
endif()

if(NOT DEFINED CMAKE_PREFIX_PATH)
	set(CMAKE_PREFIX_PATH "${QTDIR}")
endif()

# Resolves QT_VERSION_MAJOR (needed by get_target_name() below) regardless of whether 
# the caller's own, fuller find_package(Qt...) call happens before or after including this file.
if(NOT DEFINED QT_VERSION_MAJOR)
	find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
endif()

if(NOT DEFINED ACFCONFIGDIR)
	set(ACFCONFIGDIR "$ENV{ACFCONFIGDIR}")
endif()

if(NOT DEFINED ACFDIR_BUILD)
	set(ACFDIR_BUILD "$ENV{ACFDIR_BUILD}")
	if(ACFDIR_BUILD STREQUAL "")
		set(ACFDIR_BUILD ${ACFDIR})
	endif()
endif()

file(TO_CMAKE_PATH "${ACFDIR_BUILD}" ACFDIR_BUILD)

if(NOT DEFINED OPENCVDIR_4_5_3)
	set(OPENCVDIR_4_5_3 "${ACFDIR}/../3rdParty/OpenCV/4.5.3")
endif()

include("${ACFDIR}/Config/CMake/ProjectRoot.cmake")

get_target_name(TARGETNAME)

# Unified modern link-scope defaults for repositories that opt into
# target-based CMake (set ACF_MODERN_CMAKE ON before including AcfEnv.cmake).
if(ACF_MODERN_CMAKE)
	define_link_scope_var(ACF_QT_MODULE_LINK_SCOPE "PRIVATE" "Link scope used for Qt module dependencies")
	define_link_scope_var(ACF_LIBRARY_LINK_SCOPE "PUBLIC" "Link scope used by inter-library dependencies")
	define_link_scope_var(ACF_PACKAGE_LINK_SCOPE "PRIVATE" "Link scope used by package (Pck) libraries linking their dependencies")
	define_link_scope_var(ACF_APPLICATION_LINK_SCOPE "PRIVATE" "Link scope used by executables linking their dependencies")
endif()

if(DEFINED ENV{ACFDIR_BUILD})
	set(ACFDIR_BUILD "$ENV{ACFDIR_BUILD}")
else()
	set(ACFDIR_BUILD ${ACFDIR})
endif()

# AuxInclude for generated files — always needed (build-tree-specific, not
# carried by imported targets).
include_directories("${ACFDIR_BUILD}/AuxInclude/${TARGETNAME}")

# Legacy mode: global include/link dirs for repos that haven't migrated to
# find_package(Acf) + target-based deps yet. Skipped when ACF_MODERN_CMAKE is ON.
if(NOT ACF_MODERN_CMAKE)
	include_directories("${ACFDIR}/Include")
	include_directories("${ACFDIR}/Impl")
	link_directories(${ACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})

	message(VERBOSE "Acf link_directories ${ACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}")
endif()

