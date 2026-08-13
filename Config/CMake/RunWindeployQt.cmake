# Runs windeployqt under a build tree wide lock.
#
# Many targets deploy into the same Bin/<config>_<target> directory, and windeployqt has no
# locking of its own: it checks whether a plugin directory exists, then creates it, then copies
# the files. Concurrent POST_BUILD runs therefore fail sporadically with messages like
# "Cannot create generic." or "Cannot copy ...: Destination file exists".
#
# Expected definitions:
#   TOOL      - path to windeployqt
#   ARGS      - arguments passed to the tool (semicolon separated list)
#   LOCK_FILE - lock file shared by all deployment steps of the build tree

if(NOT DEFINED TOOL OR NOT DEFINED LOCK_FILE)
	message(FATAL_ERROR "RunWindeployQt.cmake: TOOL and LOCK_FILE must be defined")
endif()

# Call sites wrap the paths in literal quote characters, they must not reach the tool.
string(REPLACE "\"" "" ARGS "${ARGS}")

file(LOCK "${LOCK_FILE}" GUARD PROCESS TIMEOUT 900 RESULT_VARIABLE lockResult)
if(NOT lockResult STREQUAL "0")
	message(FATAL_ERROR "Cannot lock ${LOCK_FILE}: ${lockResult}")
endif()

# OUTPUT_QUIET skips the spam output (dll up to date etc.), errors and warnings are kept.
execute_process(COMMAND "${TOOL}" ${ARGS} OUTPUT_QUIET RESULT_VARIABLE deployResult)

file(LOCK "${LOCK_FILE}" RELEASE)

if(NOT deployResult STREQUAL "0")
	message(FATAL_ERROR "windeployqt failed (${deployResult}): ${TOOL} ${ARGS}")
endif()
