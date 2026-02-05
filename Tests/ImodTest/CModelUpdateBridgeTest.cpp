// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CModelUpdateBridgeTest.h"

#include <itest/CAutoAdd.h>


void CModelUpdateBridgeTest::initTestCase()
{
	m_targetChangeable = new TargetChangeable();
	m_bridge = new imod::CModelUpdateBridge(m_targetChangeable);
	m_model1 = new TestModel();
	m_model2 = new TestModel();
}


void CModelUpdateBridgeTest::cleanupTestCase()
{
	delete m_model2;
	delete m_model1;
	delete m_bridge;
	delete m_targetChangeable;
}


void CModelUpdateBridgeTest::testBridgeCreation()
{
	// Test creating bridge with different update flags
	TargetChangeable target;
	
	imod::CModelUpdateBridge bridge1(&target, imod::CModelUpdateBridge::UF_DELEGATED);
	QCOMPARE(bridge1.GetModelCount(), 0);

	imod::CModelUpdateBridge bridge2(&target, imod::CModelUpdateBridge::UF_SOURCE);
	QCOMPARE(bridge2.GetModelCount(), 0);

	imod::CModelUpdateBridge bridge3(&target, imod::CModelUpdateBridge::UF_DELEGATED | imod::CModelUpdateBridge::UF_SOURCE);
	QCOMPARE(bridge3.GetModelCount(), 0);
}


void CModelUpdateBridgeTest::testAttachModelToBridge()
{
	// Attach model to bridge
	QVERIFY(m_model1->AttachObserver(m_bridge));
	QCOMPARE(m_bridge->GetModelCount(), 1);
	QVERIFY(m_bridge->IsModelAttached(m_model1));

	// Cleanup
	m_model1->DetachObserver(m_bridge);
}


void CModelUpdateBridgeTest::testDetachModelFromBridge()
{
	// Attach then detach
	m_model1->AttachObserver(m_bridge);
	QCOMPARE(m_bridge->GetModelCount(), 1);

	m_model1->DetachObserver(m_bridge);
	QCOMPARE(m_bridge->GetModelCount(), 0);
	QVERIFY(!m_bridge->IsModelAttached(m_model1));
}


void CModelUpdateBridgeTest::testEnsureModelsDetached()
{
	// Attach multiple models
	m_model1->AttachObserver(m_bridge);
	m_model2->AttachObserver(m_bridge);
	QCOMPARE(m_bridge->GetModelCount(), 2);

	// Detach all
	m_bridge->EnsureModelsDetached();
	QCOMPARE(m_bridge->GetModelCount(), 0);
	QVERIFY(!m_bridge->IsModelAttached(m_model1));
	QVERIFY(!m_bridge->IsModelAttached(m_model2));

	// Call again when empty - should not crash
	m_bridge->EnsureModelsDetached();
	QCOMPARE(m_bridge->GetModelCount(), 0);
}


void CModelUpdateBridgeTest::testGetModelCount()
{
	// Start with no models
	QCOMPARE(m_bridge->GetModelCount(), 0);

	// Add models
	m_model1->AttachObserver(m_bridge);
	QCOMPARE(m_bridge->GetModelCount(), 1);

	m_model2->AttachObserver(m_bridge);
	QCOMPARE(m_bridge->GetModelCount(), 2);

	// Remove models
	m_model1->DetachObserver(m_bridge);
	QCOMPARE(m_bridge->GetModelCount(), 1);

	// Cleanup
	m_bridge->EnsureModelsDetached();
}


void CModelUpdateBridgeTest::testGetObservedModel()
{
	// Attach models
	m_model1->AttachObserver(m_bridge);
	m_model2->AttachObserver(m_bridge);

	// Get models by index
	QCOMPARE(m_bridge->GetObservedModel(0), m_model1);
	QCOMPARE(m_bridge->GetObservedModel(1), m_model2);

	// Test out of range
	QVERIFY(m_bridge->GetObservedModel(2) == nullptr);
	QVERIFY(m_bridge->GetObservedModel(-1) == nullptr);

	// Cleanup
	m_bridge->EnsureModelsDetached();
}


void CModelUpdateBridgeTest::testDelegatedUpdateFlag()
{
	// Create bridge with DELEGATED flag
	TargetChangeable target;
	imod::CModelUpdateBridge bridge(&target, imod::CModelUpdateBridge::UF_DELEGATED);

	// Attach model
	m_model1->AttachObserver(&bridge);
	target.Reset();

	// Make a change to model
	m_model1->SetData(100);

	// Verify target was notified with CF_DELEGATED flag
	QCOMPARE(target.GetChangeCount(), 1);
	QVERIFY(target.GetLastChangeSet().Contains(istd::IChangeable::CF_DELEGATED));

	// The source change flags should NOT be in the change set
	QVERIFY(!target.GetLastChangeSet().Contains(TestDataModel::CF_DATA_CHANGED));

	// Cleanup
	m_model1->DetachObserver(&bridge);
}


void CModelUpdateBridgeTest::testSourceUpdateFlag()
{
	// Create bridge with SOURCE flag
	TargetChangeable target;
	imod::CModelUpdateBridge bridge(&target, imod::CModelUpdateBridge::UF_SOURCE);

	// Attach model
	m_model1->AttachObserver(&bridge);
	target.Reset();

	// Make a change to model
	m_model1->SetData(200);

	// Verify target was notified with source change flags
	QCOMPARE(target.GetChangeCount(), 1);
	QVERIFY(target.GetLastChangeSet().Contains(TestDataModel::CF_DATA_CHANGED));

	// The CF_DELEGATED flag should NOT be set
	QVERIFY(!target.GetLastChangeSet().Contains(istd::IChangeable::CF_DELEGATED));

	// Cleanup
	m_model1->DetachObserver(&bridge);
}


void CModelUpdateBridgeTest::testMultipleModelsUpdate()
{
	// Attach multiple models to bridge
	m_model1->AttachObserver(m_bridge);
	m_model2->AttachObserver(m_bridge);
	m_targetChangeable->Reset();

	// Make changes to first model
	m_model1->SetData(300);
	QCOMPARE(m_targetChangeable->GetChangeCount(), 1);

	// Make changes to second model
	m_model2->SetStatus(400);
	QCOMPARE(m_targetChangeable->GetChangeCount(), 2);

	// Make more changes
	m_model1->SetStatus(500);
	m_model2->SetData(600);
	QCOMPARE(m_targetChangeable->GetChangeCount(), 4);

	// Cleanup
	m_bridge->EnsureModelsDetached();
	m_targetChangeable->Reset();
}


void CModelUpdateBridgeTest::testBridgeWithNullChangeable()
{
	// Create bridge with null changeable - should not crash
	imod::CModelUpdateBridge bridge(nullptr);

	// Attach model
	QVERIFY(m_model1->AttachObserver(&bridge));
	QCOMPARE(bridge.GetModelCount(), 1);

	// Make changes - should not crash
	m_model1->SetData(700);

	// Cleanup
	m_model1->DetachObserver(&bridge);
}


void CModelUpdateBridgeTest::testMultipleAttachDetachCycles()
{
	// Perform multiple attach/detach cycles
	for (int i = 0; i < 5; ++i)
	{
		// Attach
		m_model1->AttachObserver(m_bridge);
		m_model2->AttachObserver(m_bridge);
		QCOMPARE(m_bridge->GetModelCount(), 2);

		m_targetChangeable->Reset();

		// Make changes
		m_model1->SetData(i * 100);
		m_model2->SetStatus(i * 200);
		QCOMPARE(m_targetChangeable->GetChangeCount(), 2);

		// Detach
		m_bridge->EnsureModelsDetached();
		QCOMPARE(m_bridge->GetModelCount(), 0);
	}
}


I_ADD_TEST(CModelUpdateBridgeTest);
