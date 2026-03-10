// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CModelProxyTest.h"

#include <itest/CAutoAdd.h>


void CModelProxyTest::initTestCase()
{
	m_proxy = new imod::CModelProxy();
	m_model1 = new TestModel();
	m_model2 = new TestModel();
	m_observer = new TestObserver();

	// Initialize models with different values
	m_model1->SetValue(100);
	m_model2->SetValue(200);
}


void CModelProxyTest::cleanupTestCase()
{
	delete m_observer;
	delete m_model2;
	delete m_model1;
	delete m_proxy;
}


void CModelProxyTest::testProxyCreation()
{
	// Test that proxy can be created
	imod::CModelProxy proxy;
	
	// Initially no model should be set
	QVERIFY(!proxy.IsAttached(m_observer));
}


void CModelProxyTest::testSetModelPtr()
{
	// Set a model through the proxy
	m_proxy->SetModelPtr(m_model1);

	// Verify we can attach observers through proxy
	QVERIFY(m_proxy->AttachObserver(m_observer));
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Cleanup
	m_proxy->DetachObserver(m_observer);
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testResetModel()
{
	// Set a model
	m_proxy->SetModelPtr(m_model1);
	m_proxy->AttachObserver(m_observer);

	// Reset the model
	m_proxy->ResetModel();

	// Observer should still be attached to proxy, but no underlying model
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Cleanup
	m_proxy->DetachAllObservers();
	m_observer->Reset();
}


void CModelProxyTest::testObserverAttachment()
{
	// Attach observer to proxy without model
	QVERIFY(m_proxy->AttachObserver(m_observer));
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Set model after observer is attached
	m_proxy->SetModelPtr(m_model1);
	
	// Observer should still be attached
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testModelSwitching()
{
	// Set initial model
	m_proxy->SetModelPtr(m_model1);
	m_proxy->AttachObserver(m_observer);
	m_observer->Reset();

	// Change model1 - should trigger notification
	m_model1->SetValue(150);
	QCOMPARE(m_observer->GetUpdateCount(), 1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_VALUE_CHANGED));

	// Switch to model2
	m_observer->Reset();
	m_proxy->SetModelPtr(m_model2);

	// Change model1 - should NOT trigger notification anymore
	m_model1->SetValue(175);
	QCOMPARE(m_observer->GetUpdateCount(), 0);

	// Change model2 - should trigger notification
	m_model2->SetValue(250);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testNotificationForwarding()
{
	// Set up proxy with model and observer
	m_proxy->SetModelPtr(m_model1);
	m_proxy->AttachObserver(m_observer);
	m_observer->Reset();

	// Make changes to underlying model
	m_model1->SetValue(300);

	// Verify notifications are forwarded through proxy
	QCOMPARE(m_observer->GetUpdateCount(), 1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(TestDataModel::CF_VALUE_CHANGED));

	// Make more changes
	m_model1->SetValue(400);
	QCOMPARE(m_observer->GetUpdateCount(), 2);

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testObserverPersistenceAcrossSwitches()
{
	// Attach observer to proxy
	m_proxy->AttachObserver(m_observer);
	m_observer->Reset();

	// Set first model
	m_proxy->SetModelPtr(m_model1);
	m_model1->SetValue(500);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Switch to second model - observer should remain attached
	m_observer->Reset();
	m_proxy->SetModelPtr(m_model2);
	m_model2->SetValue(600);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Switch back to first model
	m_observer->Reset();
	m_proxy->SetModelPtr(m_model1);
	m_model1->SetValue(700);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testDetachObserverFromProxy()
{
	// Set up proxy with model and observer
	m_proxy->SetModelPtr(m_model1);
	m_proxy->AttachObserver(m_observer);
	m_observer->Reset();

	// Verify observer is attached
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Detach observer
	m_proxy->DetachObserver(m_observer);
	QVERIFY(!m_proxy->IsAttached(m_observer));

	// Changes should not trigger notifications
	m_model1->SetValue(800);
	QCOMPARE(m_observer->GetUpdateCount(), 0);

	// Cleanup
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testSwitchToNullModel()
{
	// Set up proxy with model and observer
	m_proxy->SetModelPtr(m_model1);
	m_proxy->AttachObserver(m_observer);
	m_observer->Reset();

	// Verify notifications work
	m_model1->SetValue(900);
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Switch to null model
	m_observer->Reset();
	m_proxy->SetModelPtr(nullptr);

	// Changes to old model should not trigger notifications
	m_model1->SetValue(1000);
	QCOMPARE(m_observer->GetUpdateCount(), 0);

	// Observer should still be attached to proxy
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testMultipleObservers()
{
	TestObserver observer2, observer3;

	// Attach multiple observers to proxy
	m_proxy->AttachObserver(m_observer);
	m_proxy->AttachObserver(&observer2);
	m_proxy->AttachObserver(&observer3);

	QVERIFY(m_proxy->IsAttached(m_observer));
	QVERIFY(m_proxy->IsAttached(&observer2));
	QVERIFY(m_proxy->IsAttached(&observer3));

	// Set model
	m_proxy->SetModelPtr(m_model1);
	m_observer->Reset();
	observer2.Reset();
	observer3.Reset();

	// Make a change - all observers should be notified
	m_model1->SetValue(1100);
	QCOMPARE(m_observer->GetUpdateCount(), 1);
	QCOMPARE(observer2.GetUpdateCount(), 1);
	QCOMPARE(observer3.GetUpdateCount(), 1);

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


void CModelProxyTest::testResetWithObservers()
{
	// Set up proxy with model and observers
	m_proxy->SetModelPtr(m_model1);
	m_proxy->AttachObserver(m_observer);
	m_observer->Reset();

	// Reset model while observers are attached
	m_proxy->ResetModel();

	// Observer should still be attached to proxy
	QVERIFY(m_proxy->IsAttached(m_observer));

	// Set a new model
	m_proxy->SetModelPtr(m_model2);
	m_model2->SetValue(1200);

	// Observer should still receive notifications
	QCOMPARE(m_observer->GetUpdateCount(), 1);

	// Cleanup
	m_proxy->DetachAllObservers();
	m_proxy->ResetModel();
	m_observer->Reset();
}


I_ADD_TEST(CModelProxyTest);
