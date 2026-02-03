// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CMultiModelDispatcherBaseTest.h"

#include <itest/CAutoAdd.h>


void CMultiModelDispatcherBaseTest::initTestCase()
{
	m_dispatcher = new TestDispatcher();
	m_model1 = new TestModel();
	m_model2 = new TestModel();
	m_model3 = new TestModel();
}


void CMultiModelDispatcherBaseTest::cleanupTestCase()
{
	delete m_model3;
	delete m_model2;
	delete m_model1;
	delete m_dispatcher;
}


void CMultiModelDispatcherBaseTest::testRegisterModel()
{
	// Register a model with ID
	QVERIFY(m_dispatcher->RegisterModel(m_model1, 1));

	// Verify GetObjectAt returns correct model
	TestModel* retrieved = m_dispatcher->GetObjectAt<TestModel>(1);
	QCOMPARE(retrieved, m_model1);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testUnregisterModel()
{
	// Register and then unregister
	m_dispatcher->RegisterModel(m_model1, 1);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(1) != nullptr);

	m_dispatcher->UnregisterModel(1);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(1) == nullptr);
}


void CMultiModelDispatcherBaseTest::testUnregisterAllModels()
{
	// Register multiple models
	m_dispatcher->RegisterModel(m_model1, 1);
	m_dispatcher->RegisterModel(m_model2, 2);
	m_dispatcher->RegisterModel(m_model3, 3);

	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(1) != nullptr);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(2) != nullptr);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(3) != nullptr);

	// Unregister all
	m_dispatcher->UnregisterAllModels();

	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(1) == nullptr);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(2) == nullptr);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(3) == nullptr);
}


void CMultiModelDispatcherBaseTest::testGetObjectAt()
{
	// Register models with different IDs
	m_dispatcher->RegisterModel(m_model1, 10);
	m_dispatcher->RegisterModel(m_model2, 20);
	m_dispatcher->RegisterModel(m_model3, 30);

	// Test retrieval by ID
	QCOMPARE(m_dispatcher->GetObjectAt<TestModel>(10), m_model1);
	QCOMPARE(m_dispatcher->GetObjectAt<TestModel>(20), m_model2);
	QCOMPARE(m_dispatcher->GetObjectAt<TestModel>(30), m_model3);

	// Test non-existent ID
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(99) == nullptr);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testOnModelChanged()
{
	// Register model
	m_dispatcher->RegisterModel(m_model1, 1);
	m_dispatcher->Reset();

	// Make a change
	m_model1->SetValue(100);

	// Verify dispatcher was notified
	QCOMPARE(m_dispatcher->GetNotificationCount(), 1);
	QCOMPARE(m_dispatcher->GetLastModelId(), 1);
	QVERIFY(m_dispatcher->GetLastChangeSet().Contains(TestDataModel::CF_VALUE_CHANGED));

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testMultipleModelNotifications()
{
	// Register multiple models
	m_dispatcher->RegisterModel(m_model1, 1);
	m_dispatcher->RegisterModel(m_model2, 2);
	m_dispatcher->RegisterModel(m_model3, 3);
	m_dispatcher->Reset();

	// Make changes to different models
	m_model1->SetValue(100);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 1);
	QCOMPARE(m_dispatcher->GetLastModelId(), 1);

	m_model2->SetState(200);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 2);
	QCOMPARE(m_dispatcher->GetLastModelId(), 2);

	m_model3->SetValue(300);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 3);
	QCOMPARE(m_dispatcher->GetLastModelId(), 3);

	// Verify change history
	const QVector<TestDispatcher::ChangeRecord>& history = m_dispatcher->GetChangeHistory();
	QCOMPARE(history.size(), 3);
	QCOMPARE(history[0].modelId, 1);
	QCOMPARE(history[1].modelId, 2);
	QCOMPARE(history[2].modelId, 3);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testRelevantFlagsFiltering()
{
	// Register model with specific relevant flags (only VALUE_CHANGED)
	istd::IChangeable::ChangeSet relevantFlags;
	relevantFlags.Set(TestDataModel::CF_VALUE_CHANGED);
	
	m_dispatcher->RegisterModel(m_model1, 1, relevantFlags);
	m_dispatcher->Reset();

	// Change value - should trigger notification
	m_model1->SetValue(100);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 1);
	QVERIFY(m_dispatcher->GetLastChangeSet().Contains(TestDataModel::CF_VALUE_CHANGED));

	// Change state - should NOT trigger notification
	m_dispatcher->Reset();
	m_model1->SetState(200);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 0);

	// Change value again - should trigger notification
	m_model1->SetValue(300);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 1);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testModelIdDispatching()
{
	// Register models with specific IDs
	m_dispatcher->RegisterModel(m_model1, 100);
	m_dispatcher->RegisterModel(m_model2, 200);
	m_dispatcher->RegisterModel(m_model3, 300);
	m_dispatcher->Reset();

	// Make changes and verify correct IDs are dispatched
	m_model1->SetValue(10);
	QCOMPARE(m_dispatcher->GetLastModelId(), 100);

	m_model2->SetValue(20);
	QCOMPARE(m_dispatcher->GetLastModelId(), 200);

	m_model3->SetValue(30);
	QCOMPARE(m_dispatcher->GetLastModelId(), 300);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testRegisterSameIdTwice()
{
	// Register first model with ID 1
	m_dispatcher->RegisterModel(m_model1, 1);
	QCOMPARE(m_dispatcher->GetObjectAt<TestModel>(1), m_model1);

	// Register second model with same ID - should replace first
	m_dispatcher->RegisterModel(m_model2, 1);
	QCOMPARE(m_dispatcher->GetObjectAt<TestModel>(1), m_model2);

	m_dispatcher->Reset();

	// Verify model1 changes don't trigger notifications
	m_model1->SetValue(100);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 0);

	// Verify model2 changes trigger notifications
	m_model2->SetValue(200);
	QCOMPARE(m_dispatcher->GetNotificationCount(), 1);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testUnregisterNonExistentModel()
{
	// Register a model
	m_dispatcher->RegisterModel(m_model1, 1);

	// Try to unregister non-existent ID - should not crash
	m_dispatcher->UnregisterModel(999);

	// Original model should still be registered
	QCOMPARE(m_dispatcher->GetObjectAt<TestModel>(1), m_model1);

	// Cleanup
	m_dispatcher->UnregisterAllModels();
}


void CMultiModelDispatcherBaseTest::testEmptyDispatcher()
{
	// Test operations on empty dispatcher
	m_dispatcher->UnregisterAllModels();

	// Should not crash
	m_dispatcher->UnregisterModel(1);
	QVERIFY(m_dispatcher->GetObjectAt<TestModel>(1) == nullptr);

	// Reset should not crash
	m_dispatcher->Reset();
	QCOMPARE(m_dispatcher->GetNotificationCount(), 0);
}


I_ADD_TEST(CMultiModelDispatcherBaseTest);
