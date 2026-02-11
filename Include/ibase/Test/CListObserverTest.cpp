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
	
	// Create an element added change set
	istd::IChangeable::ChangeSet changes = ibase::CObservableListBase::ElementAddChanges(testIndex);
	
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
	
	// Create an element removed change set
	istd::IChangeable::ChangeSet changes = ibase::CObservableListBase::ElementRemoveChanges(testIndex);
	
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
	
	// Create an element updated change set
	istd::IChangeable::ChangeSet changes = ibase::CObservableListBase::ElementUpdatedChanges(testIndex);
	
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
	observer.OnUpdate(ibase::CObservableListBase::ElementAddChanges(1));
	QCOMPARE(observer.m_lastAddedIndex, qsizetype(1));
	
	observer.Reset();
	observer.OnUpdate(ibase::CObservableListBase::ElementUpdatedChanges(2));
	QCOMPARE(observer.m_lastUpdatedIndex, qsizetype(2));
	
	observer.Reset();
	observer.OnUpdate(ibase::CObservableListBase::ElementRemoveChanges(3));
	QCOMPARE(observer.m_lastRemovedIndex, qsizetype(3));
}
