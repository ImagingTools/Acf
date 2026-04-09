// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/CListObserverTest.h>


// ACF includes
#include <ibase/CObservableListBase.h>


void CListObserverTest::initTestCase()
{
	// Setup
}


void CListObserverTest::cleanupTestCase()
{
	// Cleanup
}


// Routing tests

void CListObserverTest::testOnUpdateWithReset()
{
	CTestListObserver observer;
	
	// Create a reset change set
	istd::IChangeable::ChangeSet changes(ibase::CObservableListBase::CF_RESET);
	
	// Call OnUpdate
	observer.OnUpdate(changes);
	
	// Verify OnListReset was called
	QVERIFY(observer.m_resetCalled);
	QCOMPARE(observer.m_lastRemovedIndex, qsizetype(-1));
	QCOMPARE(observer.m_lastAddedIndex, qsizetype(-1));
	QCOMPARE(observer.m_lastUpdatedIndex, qsizetype(-1));
}


void CListObserverTest::testOnUpdateWithElementAdded()
{
	CTestListObserver observer;
	qsizetype testIndex = 5;
	
	// Create an element added change set manually
	istd::IChangeable::ChangeSet changes(ibase::CObservableListBase::CF_ELEMENT_ADDED);
	changes.SetChangeInfo(ibase::CObservableListBase::CN_INDEX_ID, testIndex);
	
	// Call OnUpdate
	observer.OnUpdate(changes);
	
	// Verify OnAfterElementAdded was called with correct index
	QVERIFY(!observer.m_resetCalled);
	QCOMPARE(observer.m_lastAddedIndex, testIndex);
	QCOMPARE(observer.m_lastRemovedIndex, qsizetype(-1));
	QCOMPARE(observer.m_lastUpdatedIndex, qsizetype(-1));
}


void CListObserverTest::testOnUpdateWithElementRemoved()
{
	CTestListObserver observer;
	qsizetype testIndex = 3;
	
	// Create an element removed change set manually
	istd::IChangeable::ChangeSet changes(ibase::CObservableListBase::CF_ELEMENT_REMOVED);
	changes.SetChangeInfo(ibase::CObservableListBase::CN_INDEX_ID, testIndex);
	
	// Call OnUpdate
	observer.OnUpdate(changes);
	
	// Verify OnBeforeElementRemoved was called with correct index
	QVERIFY(!observer.m_resetCalled);
	QCOMPARE(observer.m_lastRemovedIndex, testIndex);
	QCOMPARE(observer.m_lastAddedIndex, qsizetype(-1));
	QCOMPARE(observer.m_lastUpdatedIndex, qsizetype(-1));
}


void CListObserverTest::testOnUpdateWithElementUpdated()
{
	CTestListObserver observer;
	qsizetype testIndex = 7;
	
	// Create an element updated change set manually
	istd::IChangeable::ChangeSet changes(ibase::CObservableListBase::CF_ELEMENT_UPDATED);
	changes.SetChangeInfo(ibase::CObservableListBase::CN_INDEX_ID, testIndex);
	
	// Call OnUpdate
	observer.OnUpdate(changes);
	
	// Verify OnAfterElementUpdated was called with correct index
	QVERIFY(!observer.m_resetCalled);
	QCOMPARE(observer.m_lastUpdatedIndex, testIndex);
	QCOMPARE(observer.m_lastRemovedIndex, qsizetype(-1));
	QCOMPARE(observer.m_lastAddedIndex, qsizetype(-1));
}


void CListObserverTest::testMultipleNotifications()
{
	CTestListObserver observer;
	
	// Test sequence: add, update, remove
	observer.Reset();
	istd::IChangeable::ChangeSet addChanges(ibase::CObservableListBase::CF_ELEMENT_ADDED);
	addChanges.SetChangeInfo(ibase::CObservableListBase::CN_INDEX_ID, qsizetype(1));
	observer.OnUpdate(addChanges);
	QCOMPARE(observer.m_lastAddedIndex, qsizetype(1));
	
	observer.Reset();
	istd::IChangeable::ChangeSet updateChanges(ibase::CObservableListBase::CF_ELEMENT_UPDATED);
	updateChanges.SetChangeInfo(ibase::CObservableListBase::CN_INDEX_ID, qsizetype(2));
	observer.OnUpdate(updateChanges);
	QCOMPARE(observer.m_lastUpdatedIndex, qsizetype(2));
	
	observer.Reset();
	istd::IChangeable::ChangeSet removeChanges(ibase::CObservableListBase::CF_ELEMENT_REMOVED);
	removeChanges.SetChangeInfo(ibase::CObservableListBase::CN_INDEX_ID, qsizetype(3));
	observer.OnUpdate(removeChanges);
	QCOMPARE(observer.m_lastRemovedIndex, qsizetype(3));
}
