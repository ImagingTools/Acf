// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CChangeGroupTest.h"


// protected slots

void CChangeGroupTest::initTestCase()
{
}


void CChangeGroupTest::ConstructorDestructorTest()
{
	MockChangeableForGroup mock;
	
	QCOMPARE(mock.beginChangeGroupCount, 0);
	QCOMPARE(mock.endChangeGroupCount, 0);
	
	{
		// Create change group - should call BeginChangeGroup
		istd::CChangeGroup group(&mock);
		QCOMPARE(mock.beginChangeGroupCount, 1);
		QCOMPARE(mock.endChangeGroupCount, 0);
		QVERIFY(group.IsValid());
	}
	// Destructor should call EndChangeGroup
	QCOMPARE(mock.beginChangeGroupCount, 1);
	QCOMPARE(mock.endChangeGroupCount, 1);
}


void CChangeGroupTest::IsValidTest()
{
	MockChangeableForGroup mock;
	
	// Valid when constructed with non-null pointer
	istd::CChangeGroup group(&mock);
	QVERIFY(group.IsValid());
	
	// Invalid when constructed with null pointer
	istd::CChangeGroup nullGroup(nullptr);
	QVERIFY(!nullGroup.IsValid());
}


void CChangeGroupTest::ResetTest()
{
	MockChangeableForGroup mock;
	
	istd::CChangeGroup group(&mock);
	QVERIFY(group.IsValid());
	QCOMPARE(mock.endChangeGroupCount, 0);
	
	// Reset should call EndChangeGroup and invalidate the group
	group.Reset();
	QVERIFY(!group.IsValid());
	QCOMPARE(mock.endChangeGroupCount, 1);
	
	// Second reset should do nothing
	group.Reset();
	QVERIFY(!group.IsValid());
	QCOMPARE(mock.endChangeGroupCount, 1); // Count should not change
}


void CChangeGroupTest::NullChangeableTest()
{
	// Test with null changeable pointer - should not crash
	istd::CChangeGroup nullGroup(nullptr);
	QVERIFY(!nullGroup.IsValid());
	
	// Reset on null group should not crash
	nullGroup.Reset();
	QVERIFY(!nullGroup.IsValid());
}


void CChangeGroupTest::cleanupTestCase()
{
}


I_ADD_TEST(CChangeGroupTest);
