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
# The plain target_link_libraries() signature is used deliberately: the rest of
# the ACF CMake files use the plain signature too, and CMake forbids mixing the
# plain and keyword signatures on the same target. For static libraries the
# plain signature still records the dependency in the target's link interface,
# so it propagates transitively to consumers.
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
			target_link_libraries(${target} PUBLIC ${dependency})
		endif()
	endforeach()
endfunction()

# --- Foundation -------------------------------------------------------------
acf_declare_library_dependencies(itest       istd)
acf_declare_library_dependencies(iser        istd)
acf_declare_library_dependencies(imod        iser istd)
acf_declare_library_dependencies(imath       iser istd)
acf_declare_library_dependencies(iattr       imod iser istd)
acf_declare_library_dependencies(icomp       iattr imod iser istd)
acf_declare_library_dependencies(ilog        icomp imod iser istd)
acf_declare_library_dependencies(iprm        iattr icomp ilog imod iser istd)

# --- Core data models -------------------------------------------------------
acf_declare_library_dependencies(ibase       icomp ilog imod iprm iqt iser istd)
acf_declare_library_dependencies(icmm        ibase imath imod iser istd)
acf_declare_library_dependencies(i2d         ibase icomp imath imod iprm iser istd)
acf_declare_library_dependencies(i3d         i2d imath iser)
acf_declare_library_dependencies(idoc        ibase icomp ifile ilog imod iprm iser istd)
acf_declare_library_dependencies(ifile       ibase icomp idoc ilog imod iprm iser istd)
acf_declare_library_dependencies(iimg        i2d ibase icmm icomp idoc ifile ilog imath imod iprm iser istd)
acf_declare_library_dependencies(ipackage    ibase icomp ifile ilog iprm iser istd)

# --- Qt integration and GUI -------------------------------------------------
acf_declare_library_dependencies(iqt         i2d ibase icomp ifile ilog imod iprm iser istd)
acf_declare_library_dependencies(iwidgets    iqt istd)
acf_declare_library_dependencies(iqtgui      i2d iattr ibase icomp ifile iimg ilog imod iprm iqt iser istd iwidgets)
acf_declare_library_dependencies(ifilegui    icomp ifile ilog imod iqt iqtgui istd iwidgets)
acf_declare_library_dependencies(iloggui     ibase icomp ifile ilog iprm iqt iqtgui istd iwidgets)
acf_declare_library_dependencies(iview       i2d ibase icmm icomp iimg imath imod iqt iqtgui istd iwidgets)
acf_declare_library_dependencies(iqt2d       i2d ibase icomp iimg imod iprm iqt iqtgui iser istd iview)
acf_declare_library_dependencies(iqtdoc      ibase icomp idoc ifile ifilegui imod iprm iqt iqtgui istd)
acf_declare_library_dependencies(iqtprm      ifile ifilegui imod iprm iqt iqt2d iqtgui iser istd iview iwidgets)
