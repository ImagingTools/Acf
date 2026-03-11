// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <itest/CStandardTestExecutor.h>


// Qt includes
#include <QtTest/QtTest>


namespace itest
{


int CStandardTestExecutor::RunTests(int argc, char* argv[])
{
	int ret = 0;

	TestInstances& tests = GetTests();

	bool enableConcat = false;
	QString arguments;
	int argumentPosition = 0;
	if (argc >= 3){
		for (int i = 1; i < argc; ++i){
			if (strcmp(argv[i], "-o") == 0){
				if (i + 1 < argc){
					arguments = QString::fromLatin1(argv[i + 1]);
					argumentPosition = i + 1;
					if (arguments.contains(QString("."))){
						enableConcat = true;
					}
					break;
				}
			}
		}
	}

	int index = 0;
	for (TestInstances::Iterator testIter = tests.begin(); testIter != tests.end(); ++testIter){
		// If second argument contains a filename, shown via "," or ".",
		// index will be concat to filename to avoid overriding output file.
		QByteArray overrideArgBytes;
		if (enableConcat){
			auto concatArgs = arguments.split(".");
			if (concatArgs.size() > 1){
				QString overrideArg = concatArgs.at(0);
				overrideArg += QString::number(++index);
				overrideArg += QString(".");
				overrideArg += concatArgs.at(1);
				overrideArgBytes = overrideArg.toLocal8Bit();
				argv[argumentPosition] = overrideArgBytes.data();
			}
		}
		ret += QTest::qExec(testIter->GetPtr(), argc, argv);
	}

	return ret;
}


CStandardTestExecutor::TestInstances& CStandardTestExecutor::GetTests()
{
	static TestInstances tests;

	return tests;
}


bool CStandardTestExecutor::FindTest(const QString& name)
{
	const TestInstances& tests = GetTests();

	for (TestInstances::ConstIterator testIter = tests.constBegin(); testIter != tests.constEnd(); ++testIter){
		if ((*testIter)->objectName() == name){
			return true;
		}
	}

	return false;
}


void CStandardTestExecutor::AddTest(TestPtr testPtr)
{
	if (FindTest(testPtr->objectName())){
		return;
	}

	GetTests().push_back(testPtr);
}


} // namespace itest


