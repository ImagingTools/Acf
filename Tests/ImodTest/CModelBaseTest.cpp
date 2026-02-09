// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CModelBaseTest.h"

#include <itest/CAutoAdd.h>


void CModelBaseTest::initTestCase()
{
	m_model = new TestModel();
	m_observer = new TestObserver();
}


void CModelBaseTest::cleanupTestCase()
{
	delete m_observer;
	delete m_model;
}


void CModelBaseTest::testAttachObserver()
{
	// Test attaching an observer
	QVERIFY(m_model->AttachObserver(m_observer));
	QVERIFY(m_model->IsAttached(m_observer));
	QCOMPARE(m_model->GetObserverCount(), 1);

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CModelBaseTest::testDetachObserver()
{
	// Attach observer first
	m_model->AttachObserver(m_observer);
	QVERIFY(m_model->IsAttached(m_observer));

	// Test detaching
	m_model->DetachObserver(m_observer);
	QVERIFY(!m_model->IsAttached(m_observer));
	QCOMPARE(m_model->GetObserverCount(), 0);

	m_observer->Reset();
}


void CModelBaseTest::testDetachAllObservers()
{
	// Create multiple observers
	TestObserver observer1, observer2, observer3;

	// Attach all observers
	m_model->AttachObserver(&observer1);
	m_model->AttachObserver(&observer2);
	m_model->AttachObserver(&observer3);
	QCOMPARE(m_model->GetObserverCount(), 3);

	// Detach all
	m_model->DetachAllObservers();
	QCOMPARE(m_model->GetObserverCount(), 0);
	QVERIFY(!m_model->IsAttached(&observer1));
	QVERIFY(!m_model->IsAttached(&observer2));
	QVERIFY(!m_model->IsAttached(&observer3));
}


void CModelBaseTest::testIsAttached()
{
	TestObserver observer;

	// Test when not attached
	QVERIFY(!m_model->IsAttached(&observer));

	// Test when attached
	m_model->AttachObserver(&observer);
	QVERIFY(m_model->IsAttached(&observer));

	// Test with nullptr - should return false as no observer should be attached initially
	m_model->DetachObserver(&observer);
	QVERIFY(!m_model->IsAttached(nullptr));

	// Attach observer and test nullptr again
	m_model->AttachObserver(&observer);
	QVERIFY(m_model->IsAttached(nullptr));

	// Cleanup
	m_model->DetachObserver(&observer);
}


void CModelBaseTest::testGetObserverCount()
{
	QCOMPARE(m_model->GetObserverCount(), 0);

	TestObserver observer1, observer2, observer3;

	m_model->AttachObserver(&observer1);
	QCOMPARE(m_model->GetObserverCount(), 1);

	m_model->AttachObserver(&observer2);
	QCOMPARE(m_model->GetObserverCount(), 2);

	m_model->AttachObserver(&observer3);
	QCOMPARE(m_model->GetObserverCount(), 3);

	m_model->DetachObserver(&observer2);
	QCOMPARE(m_model->GetObserverCount(), 2);

	m_model->DetachAllObservers();
	QCOMPARE(m_model->GetObserverCount(), 0);
}


void CModelBaseTest::testGetObservers()
{
	TestObserver observer1, observer2, observer3;

	// Get observers when empty
	imod::CModelBase::Observers observers = m_model->GetObservers();
	QCOMPARE(observers.size(), 0);

	// Attach observers
	m_model->AttachObserver(&observer1);
	m_model->AttachObserver(&observer2);
	m_model->AttachObserver(&observer3);

	// Get all observers
	observers = m_model->GetObservers();
	QCOMPARE(observers.size(), 3);
	QVERIFY(observers.contains(&observer1));
	QVERIFY(observers.contains(&observer2));
	QVERIFY(observers.contains(&observer3));

	// Cleanup
	m_model->DetachAllObservers();
}


void CModelBaseTest::testNotifyBeforeChange()
{
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Make a change
	m_model->SetValue(42);

	// Verify BeforeUpdate was called
	QCOMPARE(m_observer->GetBeforeUpdateCount(), 1);

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CModelBaseTest::testNotifyAfterChange()
{
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Make a change
	m_model->SetValue(42);

	// Verify AfterUpdate was called
	QCOMPARE(m_observer->GetAfterUpdateCount(), 1);

	// Verify change set contains the correct flag
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestModel::CF_VALUE_CHANGED));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CModelBaseTest::testChangeSetAccumulation()
{
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Begin group changes
	m_model->BeginGroupChanges();

	// Make multiple changes
	m_model->SetValue(100);
	m_model->SetName("TestName");

	// End group changes
	m_model->EndGroupChanges();

	// Verify change set contains both flags
	istd::IChangeable::ChangeSet changeSet = m_observer->GetLastChangeSet();
	QVERIFY(changeSet.Contains(TestModel::CF_VALUE_CHANGED));
	QVERIFY(changeSet.Contains(TestModel::CF_NAME_CHANGED));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CModelBaseTest::testMultipleObserversNotification()
{
	TestObserver observer1, observer2, observer3;

	m_model->AttachObserver(&observer1);
	m_model->AttachObserver(&observer2);
	m_model->AttachObserver(&observer3);

	// Make a change
	m_model->SetValue(123);

	// Verify all observers were notified
	QCOMPARE(observer1.GetAfterUpdateCount(), 1);
	QCOMPARE(observer2.GetAfterUpdateCount(), 1);
	QCOMPARE(observer3.GetAfterUpdateCount(), 1);

	// Verify all received correct change set
	QVERIFY(observer1.GetLastChangeSet().Contains(TestModel::CF_VALUE_CHANGED));
	QVERIFY(observer2.GetLastChangeSet().Contains(TestModel::CF_VALUE_CHANGED));
	QVERIFY(observer3.GetLastChangeSet().Contains(TestModel::CF_VALUE_CHANGED));

	// Cleanup
	m_model->DetachAllObservers();
}


void CModelBaseTest::testAttachSameObserverTwice()
{
	// Attach observer
	QVERIFY(m_model->AttachObserver(m_observer));
	QCOMPARE(m_model->GetObserverCount(), 1);

	// Try to attach same observer again - should fail
	QVERIFY(!m_model->AttachObserver(m_observer));
	QCOMPARE(m_model->GetObserverCount(), 1);

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CModelBaseTest::testDetachUnattachedObserver()
{
	TestObserver unattachedObserver;

	// Try to detach an observer that was never attached
	// This should not crash but be handled gracefully
	QVERIFY(!m_model->IsAttached(&unattachedObserver));
	m_model->DetachObserver(&unattachedObserver);  // Should not crash
	QCOMPARE(m_model->GetObserverCount(), 0);
}


void CModelBaseTest::testNotifyWithNoObservers()
{
	// Make sure no observers are attached
	m_model->DetachAllObservers();
	QCOMPARE(m_model->GetObserverCount(), 0);

	// Make a change - should not crash
	m_model->SetValue(456);
	QCOMPARE(m_model->GetValue(), 456);

	// Make multiple changes in a group - should not crash
	m_model->BeginGroupChanges();
	m_model->SetValue(789);
	m_model->SetName("NoObservers");
	m_model->EndGroupChanges();

	QCOMPARE(m_model->GetValue(), 789);
	QCOMPARE(m_model->GetName(), QString("NoObservers"));
}


I_ADD_TEST(CModelBaseTest);
