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
# Library dependencies use PUBLIC so they propagate transitively to consumers.
#
# The dependencies below are derived from the #include graph of each library.
# A couple of the libraries are mutually dependent (ibase <-> iqt and
# idoc <-> ifile); CMake explicitly allows cyclic dependencies between static
# libraries, repeating them on the final link line as needed.
#
# Included once, centrally, from Build/CMake/CMakeLists.txt after all library
# targets have been created.
# ---------------------------------------------------------------------------

# --- Foundation -------------------------------------------------------------
declare_target_dependencies(itest			istd)
declare_target_dependencies(iser			istd)
declare_target_dependencies(imod			iser)
declare_target_dependencies(imath			iser)
declare_target_dependencies(iattr			imod)
declare_target_dependencies(icomp			iattr)
declare_target_dependencies(ilog			icomp)
declare_target_dependencies(iprm			ilog)

if(${MSVC})
	declare_target_dependencies(istd		Mpr)
endif()

# --- Core data models -------------------------------------------------------
declare_target_dependencies(ibase			iprm iqt)
declare_target_dependencies(icmm			ibase imath)
declare_target_dependencies(i2d				ibase imath Qt${QT_VERSION_MAJOR}::Gui)
declare_target_dependencies(i3d				i2d)
declare_target_dependencies(idoc			ifile)
declare_target_dependencies(ifile			ibase idoc Qt${QT_VERSION_MAJOR}::Concurrent)
declare_target_dependencies(iimg			i2d icmm idoc)
declare_target_dependencies(ipackage		ifile)

# --- Qt integration and GUI -------------------------------------------------
declare_target_dependencies(iqt				i2d ifile)
declare_target_dependencies(iwidgets		iqt Qt${QT_VERSION_MAJOR}::Widgets)
declare_target_dependencies(iqtgui			iwidgets iimg)
if(QT_VERSION_MAJOR EQUAL 6)
	declare_target_dependencies(ifilegui	iqtgui Qt${QT_VERSION_MAJOR}::SvgWidgets)
else()
	declare_target_dependencies(ifilegui	iqtgui Qt${QT_VERSION_MAJOR}::Svg)
endif()
declare_target_dependencies(iloggui			iqtgui)
declare_target_dependencies(iview			iqtgui)
declare_target_dependencies(iqt2d			iview)
declare_target_dependencies(iqtdoc			ifilegui)
declare_target_dependencies(iqtprm			iqt2d ifilegui)
