# Sets CMAKE_PREFIX_PATH from QTDIR when not already defined, so find_package(Qt...) succeeds
# without a pre-set env var (e.g. on Linux CI). Include this before find_package(Qt...) and
# before any *Env.cmake (whose get_target_name() needs QT_VERSION_MAJOR already resolved).
if(NOT DEFINED QTDIR)
	file(TO_CMAKE_PATH "$ENV{QTDIR}" QTDIR)
endif()

if(NOT DEFINED CMAKE_PREFIX_PATH)
	set(CMAKE_PREFIX_PATH "${QTDIR}")
endif()
