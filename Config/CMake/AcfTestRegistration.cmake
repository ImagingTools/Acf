# Registration of the tests with CTest.
#
# The tests are registered by name, so `ctest -L <label>` runs the same tests on every platform. Labels in use by
# the products: unit - in process, no services; database - needs a database server; api - test suites against a
# running server; qml - Qt Quick Tests of the QML screens.
# A test that runs past its timeout is stopped and fails.


# add_tests(LABEL <label> TARGETS <target>... [TIMEOUT <seconds>] [WORKING_DIRECTORY <dir>])
#
# Registers test programs, one test per program, under the name of its target. TIMEOUT defaults to 300 seconds.
# Without WORKING_DIRECTORY a program runs in the build folder of CTest.
function(add_tests)
	cmake_parse_arguments(PARSE_ARGV 0 ARG "" "LABEL;TIMEOUT;WORKING_DIRECTORY" "TARGETS")
	if(NOT ARG_LABEL OR NOT ARG_TARGETS)
		message(FATAL_ERROR "add_tests: LABEL and TARGETS are required")
	endif()

	if(NOT ARG_TIMEOUT)
		set(ARG_TIMEOUT 300)
	endif()

	foreach(testTarget IN LISTS ARG_TARGETS)
		if(ARG_WORKING_DIRECTORY)
			add_test(NAME ${testTarget} COMMAND ${testTarget} WORKING_DIRECTORY "${ARG_WORKING_DIRECTORY}")
		else()
			add_test(NAME ${testTarget} COMMAND ${testTarget})
		endif()

		set_tests_properties(${testTarget} PROPERTIES LABELS ${ARG_LABEL} TIMEOUT ${ARG_TIMEOUT})
	endforeach()
endfunction()
