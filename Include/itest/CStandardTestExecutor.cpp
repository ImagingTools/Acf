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

	for (TestInstances::Iterator testIter = tests.begin(); testIter != tests.end(); ++testIter){
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


