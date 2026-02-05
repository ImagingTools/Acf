// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CSingleModelObserverBaseTest.h"

#include <itest/CAutoAdd.h>


void CSingleModelObserverBaseTest::initTestCase()
{
	m_model = new TestModel();
	m_observer = new TestObserver();
}


void CSingleModelObserverBaseTest::cleanupTestCase()
{
	delete m_observer;
	delete m_model;
}


void CSingleModelObserverBaseTest::testModelAttachment()
{
	// Test attaching observer to model
	QVERIFY(m_model->AttachObserver(m_observer));
	QVERIFY(m_observer->IsModelAttached(m_model));
	QCOMPARE(m_observer->GetObservedModel(), m_model);

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testModelDetachment()
{
	// Attach first
	m_model->AttachObserver(m_observer);
	QVERIFY(m_observer->IsModelAttached(m_model));

	// Detach
	m_model->DetachObserver(m_observer);
	QVERIFY(!m_observer->IsModelAttached(m_model));
	QVERIFY(m_observer->GetObservedModel() == nullptr);

	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testIsModelAttached()
{
	// Test with no model attached
	QVERIFY(!m_observer->IsModelAttached(m_model));
	QVERIFY(!m_observer->IsModelAttached(nullptr));

	// Attach model
	m_model->AttachObserver(m_observer);

	// Test with model attached
	QVERIFY(m_observer->IsModelAttached(m_model));
	QVERIFY(m_observer->IsModelAttached(nullptr));  // nullptr checks for any attachment

	// Test with different model pointer
	TestModel otherModel;
	QVERIFY(!m_observer->IsModelAttached(&otherModel));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testGetObservedModel()
{
	// No model attached
	QVERIFY(m_observer->GetObservedModel() == nullptr);

	// Attach model
	m_model->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetObservedModel(), m_model);

	// Detach model
	m_model->DetachObserver(m_observer);
	QVERIFY(m_observer->GetObservedModel() == nullptr);

	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testEnsureModelDetached()
{
	// Attach model
	m_model->AttachObserver(m_observer);
	QVERIFY(m_observer->IsModelAttached(m_model));

	// Use EnsureModelDetached
	m_observer->EnsureModelDetached();
	QVERIFY(!m_observer->IsModelAttached(m_model));
	QVERIFY(!m_model->IsAttached(m_observer));

	// Call again when already detached - should not crash
	m_observer->EnsureModelDetached();
	QVERIFY(!m_observer->IsModelAttached(m_model));

	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testOnUpdate()
{
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Make a change
	m_model->SetData(42);

	// Verify observer was notified
	QCOMPARE(m_observer->GetUpdateCount(), 1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_DATA_CHANGED));

	// Make another change
	m_model->SetStatus(100);
	QCOMPARE(m_observer->GetUpdateCount(), 2);
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_STATUS_CHANGED));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testBeforeAndAfterUpdate()
{
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Make a change
	m_model->SetData(42);

	// Verify both before and after callbacks were called
	QCOMPARE(m_observer->GetBeforeUpdateCount(), 1);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Make multiple changes
	m_model->SetData(100);
	m_model->SetStatus(200);

	QCOMPARE(m_observer->GetBeforeUpdateCount(), 3);
	QCOMPARE(m_observer->GetUpdateCount(), 3);

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testChangeSetFiltering()
{
	m_model->AttachObserver(m_observer);

	// Set observer to only listen for data changes
	istd::IChangeable::ChangeSet observedIds;
	observedIds.Set(TestDataModel::CF_DATA_CHANGED);
	m_observer->SetObservedIds(observedIds);

	m_observer->Reset();

	// Change data - should trigger notification
	m_model->SetData(42);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Change status - should NOT trigger notification due to filtering
	m_model->SetStatus(100);
	QCOMPARE(m_observer->GetUpdateCount(), 1);  // Still 1, not incremented

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testSetObservedIds()
{
	// Test setting observed IDs before attachment
	istd::IChangeable::ChangeSet observedIds;
	observedIds.Set(TestDataModel::CF_DATA_CHANGED);
	m_observer->SetObservedIds(observedIds);

	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Only data changes should be observed
	m_model->SetData(42);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	m_model->SetStatus(100);
	QCOMPARE(m_observer->GetUpdateCount(), 1);  // Not incremented

	// Change observed IDs while attached
	observedIds.Clear();
	observedIds.Set(TestDataModel::CF_STATUS_CHANGED);
	m_observer->SetObservedIds(observedIds);

	// Now only status changes should be observed
	m_model->SetData(200);
	QCOMPARE(m_observer->GetUpdateCount(), 1);  // Not incremented

	m_model->SetStatus(300);
	QCOMPARE(m_observer->GetUpdateCount(), 2);  // Incremented

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void CSingleModelObserverBaseTest::testMultipleAttachDetachCycles()
{
	// Perform multiple attach/detach cycles
	for (int i = 0; i < 5; ++i)
	{
		m_model->AttachObserver(m_observer);
		QVERIFY(m_observer->IsModelAttached(m_model));

		m_model->SetData(i * 10);
		QCOMPARE(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_DATA_CHANGED), true);

		m_model->DetachObserver(m_observer);
		QVERIFY(!m_observer->IsModelAttached(m_model));

		m_observer->Reset();
	}
}


void CSingleModelObserverBaseTest::testDetachNullModel()
{
	// Make sure no model is attached
	m_observer->EnsureModelDetached();
	QVERIFY(!m_observer->IsModelAttached(nullptr));

	// This should not crash
	m_observer->EnsureModelDetached();
	QVERIFY(!m_observer->IsModelAttached(nullptr));
}


I_ADD_TEST(CSingleModelObserverBaseTest);
