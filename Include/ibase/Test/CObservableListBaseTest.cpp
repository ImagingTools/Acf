// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/CObservableListBaseTest.h>


// ACF includes
#include <ibase/CObservableListBase.h>


void CObservableListBaseTest::initTestCase()
{
	// Setup
}


void CObservableListBaseTest::cleanupTestCase()
{
	// Cleanup
}


// ChangeSet generation tests

void CObservableListBaseTest::testElementAddChanges()
{
	qsizetype index = 5;
	istd::IChangeable::ChangeSet changes = ibase::CObservableListBase::ElementAddChanges(index);
	
	// Verify the change flag is set correctly
	QVERIFY(changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_ADDED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_REMOVED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_UPDATED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_RESET));
	
	// Verify the index is stored in the change set
	QVERIFY(changes.contains(ibase::CObservableListBase::CN_INDEX_ID));
	QCOMPARE(changes.value(ibase::CObservableListBase::CN_INDEX_ID).value<qsizetype>(), index);
}


void CObservableListBaseTest::testElementRemoveChanges()
{
	qsizetype index = 10;
	istd::IChangeable::ChangeSet changes = ibase::CObservableListBase::ElementRemoveChanges(index);
	
	// Verify the change flag is set correctly
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_ADDED));
	QVERIFY(changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_REMOVED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_UPDATED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_RESET));
	
	// Verify the index is stored in the change set
	QVERIFY(changes.contains(ibase::CObservableListBase::CN_INDEX_ID));
	QCOMPARE(changes.value(ibase::CObservableListBase::CN_INDEX_ID).value<qsizetype>(), index);
}


void CObservableListBaseTest::testElementUpdatedChanges()
{
	qsizetype index = 3;
	istd::IChangeable::ChangeSet changes = ibase::CObservableListBase::ElementUpdatedChanges(index);
	
	// Verify the change flag is set correctly
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_ADDED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_REMOVED));
	QVERIFY(changes.testFlag(ibase::CObservableListBase::CF_ELEMENT_UPDATED));
	QVERIFY(!changes.testFlag(ibase::CObservableListBase::CF_RESET));
	
	// Verify the index is stored in the change set
	QVERIFY(changes.contains(ibase::CObservableListBase::CN_INDEX_ID));
	QCOMPARE(changes.value(ibase::CObservableListBase::CN_INDEX_ID).value<qsizetype>(), index);
}


void CObservableListBaseTest::testChangeFlagValues()
{
	// Verify that the change flags have unique values
	QVERIFY(ibase::CObservableListBase::CF_ELEMENT_ADDED != ibase::CObservableListBase::CF_ELEMENT_REMOVED);
	QVERIFY(ibase::CObservableListBase::CF_ELEMENT_ADDED != ibase::CObservableListBase::CF_ELEMENT_UPDATED);
	QVERIFY(ibase::CObservableListBase::CF_ELEMENT_ADDED != ibase::CObservableListBase::CF_RESET);
	QVERIFY(ibase::CObservableListBase::CF_ELEMENT_REMOVED != ibase::CObservableListBase::CF_ELEMENT_UPDATED);
	QVERIFY(ibase::CObservableListBase::CF_ELEMENT_REMOVED != ibase::CObservableListBase::CF_RESET);
	QVERIFY(ibase::CObservableListBase::CF_ELEMENT_UPDATED != ibase::CObservableListBase::CF_RESET);
}


void CObservableListBaseTest::testIndexInChangeSet()
{
	// Test with different index values
	qsizetype indices[] = {0, 1, 100, 999};
	
	for (qsizetype index : indices)
	{
		istd::IChangeable::ChangeSet addChanges = ibase::CObservableListBase::ElementAddChanges(index);
		QCOMPARE(addChanges.value(ibase::CObservableListBase::CN_INDEX_ID).value<qsizetype>(), index);
		
		istd::IChangeable::ChangeSet removeChanges = ibase::CObservableListBase::ElementRemoveChanges(index);
		QCOMPARE(removeChanges.value(ibase::CObservableListBase::CN_INDEX_ID).value<qsizetype>(), index);
		
		istd::IChangeable::ChangeSet updateChanges = ibase::CObservableListBase::ElementUpdatedChanges(index);
		QCOMPARE(updateChanges.value(ibase::CObservableListBase::CN_INDEX_ID).value<qsizetype>(), index);
	}
}
