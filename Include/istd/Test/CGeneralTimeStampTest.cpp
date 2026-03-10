// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CGeneralTimeStampTest.h"

#include <QtCore/QThread>


// protected slots

void CGeneralTimeStampTest::initTestCase()
{
}


void CGeneralTimeStampTest::DefaultConstructorTest()
{
	istd::CGeneralTimeStamp timestamp;
	// After construction, timestamp should be initialized
	// Elapsed time should be valid (may be non-zero but should be reasonable)
	double elapsed = timestamp.GetElapsed();
	QVERIFY(elapsed >= 0.0);
}


void CGeneralTimeStampTest::StartTest()
{
	istd::CGeneralTimeStamp timestamp;
	
	// Start with zero elapsed time
	timestamp.Start(0.0);
	double elapsed1 = timestamp.GetElapsed();
	QVERIFY(elapsed1 >= 0.0);
	QVERIFY(elapsed1 < 0.1); // Should be very small initially
	
	// Start with specific elapsed time
	timestamp.Start(10.0);
	double elapsed2 = timestamp.GetElapsed();
	QVERIFY(elapsed2 >= 10.0);
	QVERIFY(elapsed2 < 10.1); // Should be close to 10.0
}


void CGeneralTimeStampTest::GetElapsedTest()
{
	istd::CGeneralTimeStamp timestamp;
	timestamp.Start(0.0);
	
	// Wait a small amount of time
	QThread::msleep(50); // 50 milliseconds
	
	double elapsed = timestamp.GetElapsed();
	QVERIFY(elapsed >= 0.04); // Should be at least 40ms (accounting for timing precision)
	QVERIFY(elapsed < 0.2);   // Should be less than 200ms
}


void CGeneralTimeStampTest::GetTimeToTest()
{
	istd::CGeneralTimeStamp timestamp1;
	timestamp1.Start(0.0);
	
	QThread::msleep(50); // Wait 50ms
	
	istd::CGeneralTimeStamp timestamp2;
	timestamp2.Start(0.0);
	
	// Time from timestamp1 to timestamp2 should be positive
	double timeDiff = timestamp1.GetTimeTo(timestamp2);
	QVERIFY(timeDiff >= 0.04); // Should be at least 40ms
	QVERIFY(timeDiff < 0.2);   // Should be less than 200ms
	
	// Time from timestamp2 to timestamp1 should be negative
	double timeDiffReverse = timestamp2.GetTimeTo(timestamp1);
	QVERIFY(timeDiffReverse <= -0.04);
	QVERIFY(timeDiffReverse > -0.2);
}


void CGeneralTimeStampTest::GetTimerResolutionTest()
{
	istd::CGeneralTimeStamp timestamp;
	double resolution = timestamp.GetTimerResolution();
	
	// Timer resolution should be positive and reasonable
	QVERIFY(resolution > 0.0);
	QVERIFY(resolution < 1.0); // Should be less than 1 second
}


void CGeneralTimeStampTest::WaitToTest()
{
	istd::CGeneralTimeStamp timestamp;
	timestamp.Start(0.0);
	
	// Wait until 0.05 seconds have elapsed
	timestamp.WaitTo(0.05);
	
	double elapsed = timestamp.GetElapsed();
	QVERIFY(elapsed >= 0.05);
	QVERIFY(elapsed < 0.15); // Should be close to 0.05 but allow some tolerance
}


void CGeneralTimeStampTest::cleanupTestCase()
{
}


I_ADD_TEST(CGeneralTimeStampTest);
