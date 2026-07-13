# ---------------------------------------------------------------------------
# Clean, target-based inter-library dependency graph for ACF.
#
# Historically the ACF static libraries did not declare their dependencies on
# one another: symbols were only resolved at the final executable link and the
# build order was coordinated by hand. Declaring the dependencies here as target
# usage requirements makes include paths and link order transitive and
# automatic, both for the in-tree build and for downstream consumers that use
# find_package(Acf) and link a single Acf::<lib> target.
#
# The target_link_libraries() signature is controlled by ACF_LIBRARY_LINK_SCOPE:
#  * when empty, the plain signature is used (matching legacy ACF CMake),
#  * when set to PUBLIC/PRIVATE/INTERFACE, the keyword signature is used.
# Avoid mixing plain and keyword signatures for the same target (CMake forbids it).
# For static libraries, the dependency still propagates transitively to consumers.
#
# The dependencies below are derived from the #include graph of each library.
# A couple of the libraries are mutually dependent (ibase <-> iqt and
# idoc <-> ifile); CMake explicitly allows cyclic dependencies between static
# libraries, repeating them on the final link line as needed.
#
# Included once, centrally, from Build/CMake/CMakeLists.txt after all library
# targets have been created.
# ---------------------------------------------------------------------------

# Declare the internal ACF dependencies of a library, ignoring any entry whose
# target does not exist in the current configuration (e.g. platform-specific
# or feature-gated libraries).
function(acf_declare_library_dependencies target)
	if(NOT TARGET ${target})
		return()
	endif()

	foreach(dependency IN LISTS ARGN)
		if(TARGET ${dependency})
			target_link_libraries(${target} ${ACF_LIBRARY_LINK_SCOPE} ${dependency})
		endif()
	endforeach()
endfunction()

# --- Foundation -------------------------------------------------------------
acf_declare_library_dependencies(itest		istd)
acf_declare_library_dependencies(iser		istd)
acf_declare_library_dependencies(imod		iser)
acf_declare_library_dependencies(imath		iser)
acf_declare_library_dependencies(iattr		imod)
acf_declare_library_dependencies(icomp		iattr)
acf_declare_library_dependencies(ilog		icomp)
acf_declare_library_dependencies(iprm		ilog)

if(${MSVC})
	if(TARGET istd)
		target_link_libraries(istd ${ACF_LIBRARY_LINK_SCOPE} Mpr)
	endif()
endif()

# --- Core data models -------------------------------------------------------
acf_declare_library_dependencies(ibase		iprm iqt)
acf_declare_library_dependencies(icmm		ibase imath)
acf_declare_library_dependencies(i2d		ibase imath Qt${QT_VERSION_MAJOR}::Gui)
acf_declare_library_dependencies(i3d		i2d)
acf_declare_library_dependencies(idoc		ifile)
acf_declare_library_dependencies(ifile		ibase idoc Qt${QT_VERSION_MAJOR}::Concurrent)
acf_declare_library_dependencies(iimg		i2d icmm idoc)
acf_declare_library_dependencies(ipackage	ifile)

# --- Qt integration and GUI -------------------------------------------------
acf_declare_library_dependencies(iqt		i2d ifile)
acf_declare_library_dependencies(iwidgets	iqt Qt${QT_VERSION_MAJOR}::Widgets)
acf_declare_library_dependencies(iqtgui		iwidgets iimg)
if(QT_VERSION_MAJOR EQUAL 6)
	acf_declare_library_dependencies(ifilegui	iqtgui Qt${QT_VERSION_MAJOR}::SvgWidgets)
else()
	acf_declare_library_dependencies(ifilegui	iqtgui Qt${QT_VERSION_MAJOR}::Svg)
endif()
acf_declare_library_dependencies(iloggui	iqtgui)
acf_declare_library_dependencies(iview		iqtgui)
acf_declare_library_dependencies(iqt2d		iview)
acf_declare_library_dependencies(iqtdoc		ifilegui)
acf_declare_library_dependencies(iqtprm		iqt2d ifilegui)
