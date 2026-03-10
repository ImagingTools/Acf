// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CChangeDelegatorTest.h"


// protected slots

void CChangeDelegatorTest::initTestCase()
{
}


void CChangeDelegatorTest::DefaultConstructorTest()
{
	istd::CChangeDelegator delegator;
	QVERIFY(delegator.GetSlavePtr() == nullptr);
}


void CChangeDelegatorTest::ConstructorWithSlaveTest()
{
	MockChangeable mock;
	istd::CChangeDelegator delegator(&mock);
	
	QVERIFY(delegator.GetSlavePtr() == &mock);
}


void CChangeDelegatorTest::SetGetSlavePtrTest()
{
	MockChangeable mock1;
	MockChangeable mock2;
	istd::CChangeDelegator delegator;
	
	// Initially null
	QVERIFY(delegator.GetSlavePtr() == nullptr);
	
	// Set first slave
	delegator.SetSlavePtr(&mock1);
	QVERIFY(delegator.GetSlavePtr() == &mock1);
	
	// Change to second slave
	delegator.SetSlavePtr(&mock2);
	QVERIFY(delegator.GetSlavePtr() == &mock2);
	
	// Set back to null
	delegator.SetSlavePtr(nullptr);
	QVERIFY(delegator.GetSlavePtr() == nullptr);
}


void CChangeDelegatorTest::DelegateBeginChangesTest()
{
	MockChangeable mock;
	istd::CChangeDelegator delegator(&mock);
	
	QCOMPARE(mock.beginChangesCount, 0);
	
	// Trigger begin changes on delegator
	delegator.BeginChanges(istd::IChangeable::GetAnyChange());
	
	// Verify it was delegated to the slave
	QCOMPARE(mock.beginChangesCount, 1);
	
	// Test with null slave - should not crash
	delegator.SetSlavePtr(nullptr);
	delegator.BeginChanges(istd::IChangeable::GetAnyChange());
	QCOMPARE(mock.beginChangesCount, 1); // Count shouldn't change
}


void CChangeDelegatorTest::DelegateEndChangesTest()
{
	MockChangeable mock;
	istd::CChangeDelegator delegator(&mock);
	
	QCOMPARE(mock.endChangesCount, 0);
	
	// Trigger end changes with a change set
	istd::IChangeable::ChangeSet changeSet(1, "Test change");
	delegator.EndChanges(changeSet);
	
	// Verify it was delegated to the slave
	QCOMPARE(mock.endChangesCount, 1);
	
	// Test with empty change set - should not trigger delegation
	istd::IChangeable::ChangeSet emptyChangeSet;
	delegator.EndChanges(emptyChangeSet);
	QCOMPARE(mock.endChangesCount, 1); // Count shouldn't change
	
	// Test with null slave - should not crash
	delegator.SetSlavePtr(nullptr);
	delegator.EndChanges(changeSet);
	QCOMPARE(mock.endChangesCount, 1); // Count shouldn't change
}


void CChangeDelegatorTest::cleanupTestCase()
{
}


I_ADD_TEST(CChangeDelegatorTest);
