// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CChangeNotifierTest.h"


// protected slots

void CChangeNotifierTest::initTestCase()
{
}


void CChangeNotifierTest::ConstructorDestructorTest()
{
	MockChangeableForNotifier mock;
	
	QCOMPARE(mock.beginChangesCount, 0);
	QCOMPARE(mock.endChangesCount, 0);
	
	{
		// Create change notifier - should call BeginChanges
		istd::CChangeNotifier notifier(&mock);
		QCOMPARE(mock.beginChangesCount, 1);
		QCOMPARE(mock.endChangesCount, 0);
		QVERIFY(notifier.IsValid());
	}
	// Destructor should call EndChanges
	QCOMPARE(mock.beginChangesCount, 1);
	QCOMPARE(mock.endChangesCount, 1);
}


void CChangeNotifierTest::IsValidTest()
{
	MockChangeableForNotifier mock;
	
	// Valid when constructed with non-null pointer
	istd::CChangeNotifier notifier(&mock);
	QVERIFY(notifier.IsValid());
	
	// Invalid when constructed with null pointer
	istd::CChangeNotifier nullNotifier(nullptr);
	QVERIFY(!nullNotifier.IsValid());
}


void CChangeNotifierTest::ResetTest()
{
	MockChangeableForNotifier mock;
	
	istd::CChangeNotifier notifier(&mock);
	QVERIFY(notifier.IsValid());
	QCOMPARE(mock.endChangesCount, 0);
	
	// Reset should call EndChanges and invalidate the notifier
	notifier.Reset();
	QVERIFY(!notifier.IsValid());
	QCOMPARE(mock.endChangesCount, 1);
	
	// Second reset should do nothing
	notifier.Reset();
	QVERIFY(!notifier.IsValid());
	QCOMPARE(mock.endChangesCount, 1); // Count should not change
}


void CChangeNotifierTest::AbortTest()
{
	MockChangeableForNotifier mock;
	
	istd::CChangeNotifier notifier(&mock);
	QVERIFY(notifier.IsValid());
	QCOMPARE(mock.endChangesCount, 0);
	
	// Abort should call EndChanges with no changes and invalidate the notifier
	notifier.Abort();
	QVERIFY(!notifier.IsValid());
	QCOMPARE(mock.endChangesCount, 1);
	
	// Verify that EndChanges was called with no changes
	QVERIFY(mock.lastEndChangeSet.IsEmpty());
}


void CChangeNotifierTest::NullChangeableTest()
{
	// Test with null changeable pointer - should not crash
	istd::CChangeNotifier nullNotifier(nullptr);
	QVERIFY(!nullNotifier.IsValid());
	
	// Reset on null notifier should not crash
	nullNotifier.Reset();
	QVERIFY(!nullNotifier.IsValid());
	
	// Abort on null notifier should not crash
	nullNotifier.Abort();
	QVERIFY(!nullNotifier.IsValid());
}


void CChangeNotifierTest::cleanupTestCase()
{
}


I_ADD_TEST(CChangeNotifierTest);
