#pragma once


#include <itest/CStandardTestExecutor.h>

// Qt includes
#include <QtCore/QCoreApplication>


/**
	Macro to define the standard main function for ACF test executables.
	This eliminates code duplication across test main.cpp files.
	
	Usage:
	#include <itest/TestMain.h>
	I_TEST_MAIN()
*/
#define I_TEST_MAIN() \
int main(int argc, char *argv[]) \
{ \
	QCoreApplication app(argc, argv); \
	itest::CStandardTestExecutor instance; \
	return instance.RunTests(argc, argv); \
}


