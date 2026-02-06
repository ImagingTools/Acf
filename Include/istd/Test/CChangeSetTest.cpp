// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CChangeSetTest.h"


// protected slots

void CChangeSetTest::initTestCase()
{
}


void CChangeSetTest::DefaultConstructorTest()
{
	istd::IChangeable::ChangeSet changeSet;
	QVERIFY(changeSet.IsEmpty());
	QVERIFY(changeSet.GetDescription().isEmpty());
}


void CChangeSetTest::SingleIdConstructorTest()
{
	istd::IChangeable::ChangeSet changeSet(42, "Test change");
	
	QVERIFY(!changeSet.IsEmpty());
	QVERIFY(changeSet.Contains(42));
	QVERIFY(!changeSet.Contains(99));
	QCOMPARE(changeSet.GetDescription(), QString("Test change"));
}


void CChangeSetTest::MultipleIdsConstructorTest()
{
	// Test with 2 IDs
	istd::IChangeable::ChangeSet changeSet2(1, 2, "Two changes");
	QVERIFY(changeSet2.Contains(1));
	QVERIFY(changeSet2.Contains(2));
	QVERIFY(!changeSet2.Contains(3));
	
	// Test with 3 IDs
	istd::IChangeable::ChangeSet changeSet3(10, 20, 30, "Three changes");
	QVERIFY(changeSet3.Contains(10));
	QVERIFY(changeSet3.Contains(20));
	QVERIFY(changeSet3.Contains(30));
	QVERIFY(!changeSet3.Contains(40));
	
	// Test with 5 IDs
	istd::IChangeable::ChangeSet changeSet5(1, 2, 3, 4, 5);
	QVERIFY(changeSet5.Contains(1));
	QVERIFY(changeSet5.Contains(3));
	QVERIFY(changeSet5.Contains(5));
	QVERIFY(!changeSet5.Contains(6));
}


void CChangeSetTest::DescriptionConstructorTest()
{
	istd::IChangeable::ChangeSet changeSet("Description only");
	QVERIFY(changeSet.IsEmpty());
	QCOMPARE(changeSet.GetDescription(), QString("Description only"));
}


void CChangeSetTest::IsEmptyTest()
{
	istd::IChangeable::ChangeSet emptySet;
	QVERIFY(emptySet.IsEmpty());
	
	istd::IChangeable::ChangeSet nonEmptySet(1);
	QVERIFY(!nonEmptySet.IsEmpty());
}


void CChangeSetTest::ContainsTest()
{
	istd::IChangeable::ChangeSet changeSet(1, 2, 3);
	
	QVERIFY(changeSet.Contains(1));
	QVERIFY(changeSet.Contains(2));
	QVERIFY(changeSet.Contains(3));
	QVERIFY(!changeSet.Contains(4));
	QVERIFY(!changeSet.Contains(0));
}


void CChangeSetTest::ContainsAnyTest()
{
	istd::IChangeable::ChangeSet changeSet1(1, 2, 3);
	istd::IChangeable::ChangeSet changeSet2(3, 4, 5);
	istd::IChangeable::ChangeSet changeSet3(6, 7, 8);
	
	// changeSet1 and changeSet2 share ID 3
	QVERIFY(changeSet1.ContainsAny(changeSet2));
	QVERIFY(changeSet2.ContainsAny(changeSet1));
	
	// changeSet1 and changeSet3 share no IDs
	QVERIFY(!changeSet1.ContainsAny(changeSet3));
	QVERIFY(!changeSet3.ContainsAny(changeSet1));
}


void CChangeSetTest::GetIdsTest()
{
	istd::IChangeable::ChangeSet changeSet(1, 2, 3, 4);
	QSet<int> ids = changeSet.GetIds();
	
	QVERIFY(ids.contains(1));
	QVERIFY(ids.contains(2));
	QVERIFY(ids.contains(3));
	QVERIFY(ids.contains(4));
	QVERIFY(!ids.contains(5));
}


void CChangeSetTest::ResetTest()
{
	istd::IChangeable::ChangeSet changeSet(1, 2, 3);
	QVERIFY(!changeSet.IsEmpty());
	
	changeSet.Reset();
	QVERIFY(changeSet.IsEmpty());
	QVERIFY(!changeSet.Contains(1));
	QVERIFY(!changeSet.Contains(2));
	QVERIFY(!changeSet.Contains(3));
}


void CChangeSetTest::MaskOutTest()
{
	istd::IChangeable::ChangeSet changeSet1(1, 2, 3, 4, 5);
	istd::IChangeable::ChangeSet changeSet2(2, 4);
	
	// Mask out IDs 2 and 4 from changeSet1
	changeSet1.MaskOut(changeSet2);
	
	// IDs 2 and 4 should be removed
	QVERIFY(changeSet1.Contains(1));
	QVERIFY(!changeSet1.Contains(2));
	QVERIFY(changeSet1.Contains(3));
	QVERIFY(!changeSet1.Contains(4));
	QVERIFY(changeSet1.Contains(5));
}


void CChangeSetTest::cleanupTestCase()
{
}


I_ADD_TEST(CChangeSetTest);
