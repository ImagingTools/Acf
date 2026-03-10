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
	// Test with 2 IDs (avoid ID=1 which is CF_ALL_DATA)
	istd::IChangeable::ChangeSet changeSet2(11, 12, "Two changes");
	QVERIFY(changeSet2.Contains(11));
	QVERIFY(changeSet2.Contains(12));
	QVERIFY(!changeSet2.Contains(13));
	
	// Test with 3 IDs
	istd::IChangeable::ChangeSet changeSet3(10, 20, 30, "Three changes");
	QVERIFY(changeSet3.Contains(10));
	QVERIFY(changeSet3.Contains(20));
	QVERIFY(changeSet3.Contains(30));
	QVERIFY(!changeSet3.Contains(40));
	
	// Test with 5 IDs (avoid ID=1 which is CF_ALL_DATA)
	istd::IChangeable::ChangeSet changeSet5(11, 12, 13, 14, 15);
	QVERIFY(changeSet5.Contains(11));
	QVERIFY(changeSet5.Contains(13));
	QVERIFY(changeSet5.Contains(15));
	QVERIFY(!changeSet5.Contains(16));
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
	// Note: CF_ALL_DATA = 1, so we avoid using 1 as a test ID
	istd::IChangeable::ChangeSet changeSet(10, 20, 30);
	
	QVERIFY(changeSet.Contains(10));
	QVERIFY(changeSet.Contains(20));
	QVERIFY(changeSet.Contains(30));
	QVERIFY(!changeSet.Contains(40));
	QVERIFY(!changeSet.Contains(0));
}


void CChangeSetTest::ContainsAnyTest()
{
	// Note: CF_ALL_DATA = 1, so we avoid using 1 as a test ID
	istd::IChangeable::ChangeSet changeSet1(10, 20, 30);
	istd::IChangeable::ChangeSet changeSet2(30, 40, 50);
	istd::IChangeable::ChangeSet changeSet3(60, 70, 80);
	
	// changeSet1 and changeSet2 share ID 30
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
	// Note: CF_ALL_DATA = 1, so we avoid using 1 as a test ID
	istd::IChangeable::ChangeSet changeSet1(10, 20, 30, 40, 50);
	istd::IChangeable::ChangeSet changeSet2(20, 40);
	
	// Mask out IDs 20 and 40 from changeSet1
	changeSet1.MaskOut(changeSet2);
	
	// IDs 20 and 40 should be removed
	QVERIFY(changeSet1.Contains(10));
	QVERIFY(!changeSet1.Contains(20));
	QVERIFY(changeSet1.Contains(30));
	QVERIFY(!changeSet1.Contains(40));
	QVERIFY(changeSet1.Contains(50));
}


void CChangeSetTest::cleanupTestCase()
{
}


I_ADD_TEST(CChangeSetTest);
