// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/CCumulatedProgressManagerBaseTest.h>


// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>


void CCumulatedProgressManagerBaseTest::DoOverflowTest1()
{
	ibase::CCumulatedProgressManagerBase progress;

	std::unique_ptr<ibase::IProgressLogger> loggerPtr = progress.StartProgressLogger();
	loggerPtr->OnProgress(1);

	std::unique_ptr<ibase::IProgressManager> subtaskPtr = progress.CreateSubtaskManager("subtask1", "", 0.5);
	std::unique_ptr<ibase::IProgressLogger> subtaskLoggerPtr = subtaskPtr->StartProgressLogger();
	subtaskLoggerPtr->OnProgress(1);

	double cumulatedProgress = progress.GetCumulatedProgress();

	QVERIFY(qFuzzyCompare(cumulatedProgress, 1.5));
}


I_ADD_TEST(CCumulatedProgressManagerBaseTest);


