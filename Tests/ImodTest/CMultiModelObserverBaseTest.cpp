// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CMultiModelObserverBaseTest.h"

#include <itest/CAutoAdd.h>


void CMultiModelObserverBaseTest::initTestCase()
{
	m_model1 = new TestModel();
	m_model2 = new TestModel();
	m_model3 = new TestModel();
	m_observer = new TestObserver();
}


void CMultiModelObserverBaseTest::cleanupTestCase()
{
	delete m_observer;
	delete m_model3;
	delete m_model2;
	delete m_model1;
}


void CMultiModelObserverBaseTest::testAttachMultipleModels()
{
	// Attach multiple models
	QVERIFY(m_model1->AttachObserver(m_observer));
	QCOMPARE(m_observer->GetModelCount(), 1);

	QVERIFY(m_model2->AttachObserver(m_observer));
	QCOMPARE(m_observer->GetModelCount(), 2);

	QVERIFY(m_model3->AttachObserver(m_observer));
	QCOMPARE(m_observer->GetModelCount(), 3);

	// Verify all models are attached
	QVERIFY(m_observer->IsModelAttached(m_model1));
	QVERIFY(m_observer->IsModelAttached(m_model2));
	QVERIFY(m_observer->IsModelAttached(m_model3));

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testDetachModel()
{
	// Attach models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);
	m_model3->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 3);

	// Detach middle model
	m_model2->DetachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 2);
	QVERIFY(m_observer->IsModelAttached(m_model1));
	QVERIFY(!m_observer->IsModelAttached(m_model2));
	QVERIFY(m_observer->IsModelAttached(m_model3));

	// Detach remaining models
	m_model1->DetachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 1);

	m_model3->DetachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 0);

	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testEnsureModelsDetached()
{
	// Attach multiple models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);
	m_model3->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 3);

	// Detach all at once
	m_observer->EnsureModelsDetached();
	QCOMPARE(m_observer->GetModelCount(), 0);
	QVERIFY(!m_observer->IsModelAttached(m_model1));
	QVERIFY(!m_observer->IsModelAttached(m_model2));
	QVERIFY(!m_observer->IsModelAttached(m_model3));

	// Call again when already detached - should not crash
	m_observer->EnsureModelsDetached();
	QCOMPARE(m_observer->GetModelCount(), 0);

	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testGetModelCount()
{
	// Start with no models
	QCOMPARE(m_observer->GetModelCount(), 0);

	// Add models one by one
	m_model1->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 1);

	m_model2->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 2);

	m_model3->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 3);

	// Remove models
	m_model2->DetachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 2);

	// Cleanup
	m_observer->EnsureModelsDetached();
	QCOMPARE(m_observer->GetModelCount(), 0);
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testGetObservedModel()
{
	// Attach models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);
	m_model3->AttachObserver(m_observer);

	// Get models by index
	QCOMPARE(m_observer->GetObservedModel(0), m_model1);
	QCOMPARE(m_observer->GetObservedModel(1), m_model2);
	QCOMPARE(m_observer->GetObservedModel(2), m_model3);

	// Test out of range (should return nullptr)
	QVERIFY(m_observer->GetObservedModel(3) == nullptr);
	QVERIFY(m_observer->GetObservedModel(-1) == nullptr);

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testIsModelAttached()
{
	// Test with no models attached
	QVERIFY(!m_observer->IsModelAttached(m_model1));
	QVERIFY(!m_observer->IsModelAttached(nullptr));

	// Attach one model
	m_model1->AttachObserver(m_observer);
	QVERIFY(m_observer->IsModelAttached(m_model1));
	QVERIFY(!m_observer->IsModelAttached(m_model2));
	QVERIFY(m_observer->IsModelAttached(nullptr));  // Any model attached

	// Attach more models
	m_model2->AttachObserver(m_observer);
	QVERIFY(m_observer->IsModelAttached(m_model1));
	QVERIFY(m_observer->IsModelAttached(m_model2));
	QVERIFY(!m_observer->IsModelAttached(m_model3));

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testOnUpdateFromMultipleModels()
{
	// Attach multiple models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);
	m_model3->AttachObserver(m_observer);

	m_observer->Reset();
	m_observer->ClearHistory();

	// Make changes to different models
	m_model1->SetValue(10);
	QCOMPARE(m_observer->GetUpdateCount(), 1);
	QCOMPARE(m_observer->GetLastUpdatedModel(), m_model1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_VALUE_CHANGED));

	m_model2->SetValue(20);
	QCOMPARE(m_observer->GetUpdateCount(), 2);
	QCOMPARE(m_observer->GetLastUpdatedModel(), m_model2);

	m_model3->SetState(30);
	QCOMPARE(m_observer->GetUpdateCount(), 3);
	QCOMPARE(m_observer->GetLastUpdatedModel(), m_model3);
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_STATE_CHANGED));

	// Verify update history
	const QVector<TestObserver::UpdateRecord>& history = m_observer->GetUpdateHistory();
	QCOMPARE(history.size(), 3);
	QCOMPARE(history[0].model, m_model1);
	QCOMPARE(history[1].model, m_model2);
	QCOMPARE(history[2].model, m_model3);

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testChangeSetFiltering()
{
	// Set observer to only listen for value changes
	istd::IChangeable::ChangeSet observedIds;
	observedIds.Set(TestDataModel::CF_VALUE_CHANGED);
	m_observer->SetObservedIds(observedIds);

	// Attach models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);

	m_observer->Reset();
	m_observer->ClearHistory();

	// Change value - should trigger notification
	m_model1->SetValue(100);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Change state - should NOT trigger notification
	m_model1->SetState(200);
	QCOMPARE(m_observer->GetUpdateCount(), 1);  // Still 1

	// Change value on second model - should trigger
	m_model2->SetValue(300);
	QCOMPARE(m_observer->GetUpdateCount(), 2);

	// Change state on second model - should NOT trigger
	m_model2->SetState(400);
	QCOMPARE(m_observer->GetUpdateCount(), 2);  // Still 2

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testSetObservedIds()
{
	// Attach models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);

	// Initially observe all changes
	m_observer->Reset();
	m_model1->SetValue(10);
	m_model1->SetState(20);
	QCOMPARE(m_observer->GetUpdateCount(), 2);

	// Change to only observe value changes
	m_observer->Reset();
	istd::IChangeable::ChangeSet observedIds;
	observedIds.Set(TestDataModel::CF_VALUE_CHANGED);
	m_observer->SetObservedIds(observedIds);

	m_model1->SetValue(30);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	m_model1->SetState(40);
	QCOMPARE(m_observer->GetUpdateCount(), 1);  // Not incremented

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testAttachSameModelMultipleTimes()
{
	// Attach model
	QVERIFY(m_model1->AttachObserver(m_observer));
	QCOMPARE(m_observer->GetModelCount(), 1);

	// Try to attach same model again - should fail
	QVERIFY(!m_model1->AttachObserver(m_observer));
	QCOMPARE(m_observer->GetModelCount(), 1);  // Still 1

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testDetachNonAttachedModel()
{
	// Attach only model1
	m_model1->AttachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 1);

	// Try to detach model2 which was never attached - should not crash
	m_model2->DetachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 1);  // Still 1

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


void CMultiModelObserverBaseTest::testUpdateAfterPartialDetach()
{
	// Attach all models
	m_model1->AttachObserver(m_observer);
	m_model2->AttachObserver(m_observer);
	m_model3->AttachObserver(m_observer);

	m_observer->Reset();

	// Detach middle model
	m_model2->DetachObserver(m_observer);
	QCOMPARE(m_observer->GetModelCount(), 2);

	// Make changes to all models
	m_model1->SetValue(100);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	m_model2->SetValue(200);
	QCOMPARE(m_observer->GetUpdateCount(), 1);  // Not incremented, model2 detached

	m_model3->SetValue(300);
	QCOMPARE(m_observer->GetUpdateCount(), 2);

	// Cleanup
	m_observer->EnsureModelsDetached();
	m_observer->Reset();
}


I_ADD_TEST(CMultiModelObserverBaseTest);
