# ---------------------------------------------------------------------------
# Generate and export the "Acf" CMake package.
#
# After this runs, downstream projects (AcfSln, and transitively IAcf/ImtCore)
# can discover ACF with a single call:
#
#     find_package(Acf REQUIRED)
#     target_link_libraries(myTarget PUBLIC Acf::icomp Acf::iqtgui ...)
#
# and inherit include directories and inter-library dependencies transitively.
#
# Two flavours of the package are produced:
#  * a build-tree package (no install step required), written next to the
#    compiled libraries so it matches the existing in-tree build layout, and
#  * an install-tree package for a relocatable `cmake --install` deployment.
#
# Included once, centrally, from Build/CMake/CMakeLists.txt after all library
# targets and their dependencies have been declared.
# ---------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

if(NOT DEFINED ACF_EXPORT_SET)
	set(ACF_EXPORT_SET "AcfTargets")
endif()

# Package version. project(Acf) does not set one, so fall back to a default that
# still lets consumers request a version and use find_package version checks.
if(NOT DEFINED Acf_VERSION OR Acf_VERSION STREQUAL "")
	if(DEFINED PROJECT_VERSION AND NOT PROJECT_VERSION STREQUAL "")
		set(Acf_VERSION "${PROJECT_VERSION}")
	else()
		set(Acf_VERSION "1.0.0")
	endif()
endif()

# Build-tree location: alongside the produced libraries, mirroring the existing
# Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME} layout.
set(ACF_LIB_OUTPUT_DIR "${ACFDIR_BUILD}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}")
set(ACF_PACKAGE_BUILD_DIR "${ACF_LIB_OUTPUT_DIR}/cmake")

# Ensure the output directory exists before writing export/config files into it,
# otherwise export()/configure_package_config_file() can fail on a fresh build tree.
file(MAKE_DIRECTORY "${ACF_PACKAGE_BUILD_DIR}")

# --- Build-tree export ------------------------------------------------------
export(EXPORT ${ACF_EXPORT_SET}
	NAMESPACE Acf::
	FILE "${ACF_PACKAGE_BUILD_DIR}/${ACF_EXPORT_SET}.cmake")

# Qt major version this package was built against. Baked into the generated
# AcfConfig.cmake so consumers resolve the matching Qt imported targets.
set(ACF_QT_VERSION_MAJOR "${QT_VERSION_MAJOR}")

configure_package_config_file(
	"${ACFDIR}/Config/CMake/AcfConfig.cmake.in"
	"${ACF_PACKAGE_BUILD_DIR}/AcfConfig.cmake"
	INSTALL_DESTINATION "${ACF_PACKAGE_BUILD_DIR}"
	NO_SET_AND_CHECK_MACRO)

write_basic_package_version_file(
	"${ACF_PACKAGE_BUILD_DIR}/AcfConfigVersion.cmake"
	VERSION "${Acf_VERSION}"
	COMPATIBILITY SameMajorVersion)

# Allow find_package(Acf) to locate the build-tree package directly (e.g. via
# CMAKE_PREFIX_PATH=<build>/Lib/<config>/cmake or Acf_DIR).
message(STATUS "Acf: build-tree package written to ${ACF_PACKAGE_BUILD_DIR}")

# --- Install-tree export ----------------------------------------------------
install(EXPORT ${ACF_EXPORT_SET}
	NAMESPACE Acf::
	DESTINATION "lib/cmake/Acf"
	FILE "${ACF_EXPORT_SET}.cmake")

install(FILES
	"${ACF_PACKAGE_BUILD_DIR}/AcfConfig.cmake"
	"${ACF_PACKAGE_BUILD_DIR}/AcfConfigVersion.cmake"
	DESTINATION "lib/cmake/Acf")
