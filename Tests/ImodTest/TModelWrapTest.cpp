// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TModelWrapTest.h"

#include <itest/CAutoAdd.h>


void TModelWrapTest::initTestCase()
{
	m_model = new WrappedModel();
	m_observer = new TestObserver();
}


void TModelWrapTest::cleanupTestCase()
{
	delete m_observer;
	delete m_model;
}


void TModelWrapTest::testModelWrapCreation()
{
	// Test that wrapped model can be created
	WrappedModel model;
	QCOMPARE(model.GetValue(), 0);
	QCOMPARE(model.GetText(), QString());
}


void TModelWrapTest::testSetBaseObject()
{
	// Create a base object with data
	SimpleData baseData;
	baseData.SetValue(42);
	baseData.SetText("TestData");

	// Set it in the wrapped model
	WrappedModel model;
	model.SetBaseObject(baseData);

	// Verify data was copied
	QCOMPARE(model.GetValue(), 42);
	QCOMPARE(model.GetText(), QString("TestData"));
}


void TModelWrapTest::testGetSupportedOperations()
{
	// TModelWrap should add SO_OBSERVE to base operations
	int operations = m_model->GetSupportedOperations();
	QVERIFY(operations & istd::IChangeable::SO_CLONE);
	QVERIFY(operations & istd::IChangeable::SO_COPY);
	QVERIFY(operations & istd::IChangeable::SO_RESET);
	QVERIFY(operations & istd::IChangeable::SO_OBSERVE);
}


void TModelWrapTest::testCloneMe()
{
	// Set up model with data
	m_model->SetValue(123);
	m_model->SetText("Original");

	// Clone the model
	istd::TUniqueInterfacePtr<istd::IChangeable> clonePtr = m_model->CloneMe();
	QVERIFY(clonePtr.IsValid());

	// Verify clone has same data
	WrappedModel* clonedModel = dynamic_cast<WrappedModel*>(clonePtr.Ptr());
	QVERIFY(clonedModel != nullptr);
	QCOMPARE(clonedModel->GetValue(), 123);
	QCOMPARE(clonedModel->GetText(), QString("Original"));

	// Verify clone is independent
	m_model->SetValue(456);
	QCOMPARE(clonedModel->GetValue(), 123);  // Should not change
}


void TModelWrapTest::testObserverAttachment()
{
	// Attach observer
	QVERIFY(m_model->AttachObserver(m_observer));
	QVERIFY(m_observer->IsModelAttached(m_model));

	// Detach observer
	m_model->DetachObserver(m_observer);
	QVERIFY(!m_observer->IsModelAttached(m_model));
}


void TModelWrapTest::testChangeNotification()
{
	// Attach observer
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Make a change
	m_model->SetValue(100);

	// Verify observer was notified
	QCOMPARE(m_observer->GetBeforeUpdateCount(), 1);
	QCOMPARE(m_observer->GetAfterUpdateCount(), 1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(SimpleData::CF_VALUE_CHANGED));

	// Make another change
	m_model->SetText("NewText");
	QCOMPARE(m_observer->GetBeforeUpdateCount(), 2);
	QCOMPARE(m_observer->GetAfterUpdateCount(), 2);
	QVERIFY(m_observer->GetLastChangeSet().Contains(SimpleData::CF_TEXT_CHANGED));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void TModelWrapTest::testBeginEndChanges()
{
	// Attach observer
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Manually call BeginChanges/EndChanges
	istd::IChangeable::ChangeSet changeSet;
	changeSet.Set(SimpleData::CF_VALUE_CHANGED);

	m_model->BeginChanges(changeSet);
	m_model->EndChanges(changeSet);

	// Verify notifications
	QCOMPARE(m_observer->GetBeforeUpdateCount(), 1);
	QCOMPARE(m_observer->GetAfterUpdateCount(), 1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(SimpleData::CF_VALUE_CHANGED));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void TModelWrapTest::testBeginEndChangeGroup()
{
	// Attach observer
	m_model->AttachObserver(m_observer);
	m_observer->Reset();

	// Use change groups
	istd::IChangeable::ChangeSet changeSet;
	changeSet.Set(SimpleData::CF_VALUE_CHANGED);
	changeSet.Set(SimpleData::CF_TEXT_CHANGED);

	m_model->BeginChangeGroup(changeSet);
	// Simulate multiple changes
	m_model->EndChangeGroup(changeSet);

	// Verify notifications
	QCOMPARE(m_observer->GetBeforeUpdateCount(), 1);
	QCOMPARE(m_observer->GetAfterUpdateCount(), 1);
	QVERIFY(m_observer->GetLastChangeSet().Contains(SimpleData::CF_VALUE_CHANGED));
	QVERIFY(m_observer->GetLastChangeSet().Contains(SimpleData::CF_TEXT_CHANGED));

	// Cleanup
	m_model->DetachObserver(m_observer);
	m_observer->Reset();
}


void TModelWrapTest::testModelDataAccess()
{
	// Test direct data access through wrapped model
	m_model->SetValue(999);
	QCOMPARE(m_model->GetValue(), 999);

	m_model->SetText("DirectAccess");
	QCOMPARE(m_model->GetText(), QString("DirectAccess"));

	// Verify changes are persistent
	QCOMPARE(m_model->GetValue(), 999);
	QCOMPARE(m_model->GetText(), QString("DirectAccess"));
}


void TModelWrapTest::testCopyFrom()
{
	// Create source model with data
	WrappedModel sourceModel;
	sourceModel.SetValue(777);
	sourceModel.SetText("SourceData");

	// Copy to target model
	WrappedModel targetModel;
	QVERIFY(targetModel.CopyFrom(sourceModel));

	// Verify data was copied
	QCOMPARE(targetModel.GetValue(), 777);
	QCOMPARE(targetModel.GetText(), QString("SourceData"));

	// Verify models are independent
	sourceModel.SetValue(888);
	QCOMPARE(targetModel.GetValue(), 777);  // Should not change
}


void TModelWrapTest::testResetData()
{
	// Set up model with data
	m_model->SetValue(555);
	m_model->SetText("BeforeReset");

	// Reset the model
	QVERIFY(m_model->ResetData());

	// Verify data was reset to defaults
	QCOMPARE(m_model->GetValue(), 0);
	QCOMPARE(m_model->GetText(), QString());
}


I_ADD_TEST(TModelWrapTest);
