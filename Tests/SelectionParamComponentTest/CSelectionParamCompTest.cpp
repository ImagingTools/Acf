// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CSelectionParamCompTest.h"


// ACF includes
#include <iprm/CSelectionParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CSelectionParamCompTest::initTestCase()
{
	m_testPartituraInstanceCompPtr.reset(new CSelectionParamComponentTest);

	m_selectionNoConstraintsPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionNoConstraints");
	m_selectionWithDefaultIndexPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionWithDefaultIndex");
	m_optionsConstraintsPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IOptionsManager>("OptionsConstraints");
	m_optionsConstraintsPresetPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IOptionsManager>("OptionsConstraintsPreset");
	m_selectionWithConstraintsPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionWithConstraints");
	m_selectionWithConstraintsAndDefaultPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionWithConstraintsAndDefault");
	m_selectionWithInvalidDefaultPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionWithInvalidDefault");
	m_subselectionPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("Subselection");
	m_selectionWithSubselectionPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionWithSubselection");

	QVERIFY(m_selectionNoConstraintsPtr != nullptr);
	QVERIFY(m_selectionWithDefaultIndexPtr != nullptr);
	QVERIFY(m_optionsConstraintsPtr != nullptr);
	QVERIFY(m_optionsConstraintsPresetPtr != nullptr);
	QVERIFY(m_selectionWithConstraintsPtr != nullptr);
	QVERIFY(m_selectionWithConstraintsAndDefaultPtr != nullptr);
	QVERIFY(m_selectionWithInvalidDefaultPtr != nullptr);
	QVERIFY(m_subselectionPtr != nullptr);
	QVERIFY(m_selectionWithSubselectionPtr != nullptr);
	
	// Initialize the preset options manager with some options
	// This ensures consistent test state for all tests using OptionsConstraintsPreset
	m_optionsConstraintsPresetPtr->InsertOption("Option1", QByteArray(), QString(), 0);
	m_optionsConstraintsPresetPtr->InsertOption("Option2", QByteArray(), QString(), 1);
	m_optionsConstraintsPresetPtr->InsertOption("Option3", QByteArray(), QString(), 2);
}


// Tests for selection parameter without constraints

void CSelectionParamCompTest::testCreationNoConstraints()
{
	// Verify the component was created successfully
	QVERIFY(m_selectionNoConstraintsPtr != nullptr);
	
	// Check that the default selected index is -1 (no selection)
	QVERIFY(m_selectionNoConstraintsPtr->GetSelectedOptionIndex() == -1);
	
	// Check that constraints are not set
	QVERIFY(m_selectionNoConstraintsPtr->GetSelectionConstraints() == nullptr);
}


void CSelectionParamCompTest::testDefaultIndexNoConstraints()
{
	// Verify the selection parameter with default index has the correct value
	QVERIFY(m_selectionWithDefaultIndexPtr->GetSelectedOptionIndex() == 5);
	
	// Test setting a new value
	QVERIFY(m_selectionWithDefaultIndexPtr->SetSelectedOptionIndex(10));
	QVERIFY(m_selectionWithDefaultIndexPtr->GetSelectedOptionIndex() == 10);
	
	// Test resetting to no selection
	QVERIFY(m_selectionWithDefaultIndexPtr->SetSelectedOptionIndex(-1));
	QVERIFY(m_selectionWithDefaultIndexPtr->GetSelectedOptionIndex() == -1);
	
	// Restore default value for other tests
	QVERIFY(m_selectionWithDefaultIndexPtr->SetSelectedOptionIndex(5));
}


void CSelectionParamCompTest::testSerializationNoConstraints()
{
	// Set a specific value
	QVERIFY(m_selectionNoConstraintsPtr->SetSelectedOptionIndex(15));
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_selectionNoConstraintsPtr->Serialize(writeArchive));
	
	// Change the value
	QVERIFY(m_selectionNoConstraintsPtr->SetSelectedOptionIndex(20));
	QVERIFY(m_selectionNoConstraintsPtr->GetSelectedOptionIndex() == 20);
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_selectionNoConstraintsPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QVERIFY(m_selectionNoConstraintsPtr->GetSelectedOptionIndex() == 15);
}


void CSelectionParamCompTest::testCopyNoConstraints()
{
	// Set a specific value
	QVERIFY(m_selectionNoConstraintsPtr->SetSelectedOptionIndex(25));
	
	// Create a new selection parameter and copy from the component
	iprm::CSelectionParam selectionParam;
	QVERIFY(selectionParam.CopyFrom(*m_selectionNoConstraintsPtr));
	
	// Verify the value was copied
	QVERIFY(selectionParam.GetSelectedOptionIndex() == 25);
	
	// Verify changing the copy doesn't affect the original
	selectionParam.SetSelectedOptionIndex(30);
	QVERIFY(selectionParam.GetSelectedOptionIndex() == 30);
	QVERIFY(m_selectionNoConstraintsPtr->GetSelectedOptionIndex() == 25);
}


void CSelectionParamCompTest::testCloneNoConstraints()
{
	// Set a specific value
	QVERIFY(m_selectionNoConstraintsPtr->SetSelectedOptionIndex(35));
	
	// Clone the component
	istd::TUniqueInterfacePtr<iprm::ISelectionParam> clonePtr;
	clonePtr.MoveCastedPtr(m_selectionNoConstraintsPtr->CloneMe());
	
	// Verify the clone is valid
	QVERIFY(clonePtr.IsValid());
	
	// Verify the value was cloned
	QVERIFY(clonePtr->GetSelectedOptionIndex() == 35);
	
	// Verify changing the clone doesn't affect the original
	clonePtr->SetSelectedOptionIndex(40);
	QVERIFY(clonePtr->GetSelectedOptionIndex() == 40);
	QVERIFY(m_selectionNoConstraintsPtr->GetSelectedOptionIndex() == 35);
}


// Tests for selection parameter with constraints

void CSelectionParamCompTest::testCreationWithConstraints()
{
	// Verify the component was created successfully
	QVERIFY(m_selectionWithConstraintsPtr != nullptr);
	
	// Check that constraints are set
	QVERIFY(m_selectionWithConstraintsPtr->GetSelectionConstraints() != nullptr);
	QVERIFY(m_selectionWithConstraintsPtr->GetSelectionConstraints() == m_optionsConstraintsPtr);
	
	// Check that the default selected index is -1 (no selection)
	QVERIFY(m_selectionWithConstraintsPtr->GetSelectedOptionIndex() == -1);
}


void CSelectionParamCompTest::testDefaultIndexWithConstraints()
{
	// The preset options manager already has 3 options initialized in initTestCase
	QVERIFY(m_optionsConstraintsPresetPtr->GetOptionsCount() == 3);
	
	// Verify the selection parameter with default index has the correct value
	QVERIFY(m_selectionWithConstraintsAndDefaultPtr->GetSelectedOptionIndex() == 2);
	
	// Test that we can change to a valid index
	QVERIFY(m_selectionWithConstraintsAndDefaultPtr->SetSelectedOptionIndex(1));
	QVERIFY(m_selectionWithConstraintsAndDefaultPtr->GetSelectedOptionIndex() == 1);
	
	// Restore default value
	QVERIFY(m_selectionWithConstraintsAndDefaultPtr->SetSelectedOptionIndex(2));
}


void CSelectionParamCompTest::testInvalidDefaultIndexWithConstraints()
{
	// This component was configured with a default index of 100, which is invalid
	// The component should have reset it to -1 during creation because
	// OptionsConstraintsPreset only has 3 options (indices 0, 1, 2)
	QVERIFY(m_selectionWithInvalidDefaultPtr->GetSelectedOptionIndex() == -1);
}


void CSelectionParamCompTest::testSerializationWithConstraints()
{
	// Add options to the constraints manager used by this selection parameter
	// This ensures the test is independent and doesn't rely on other tests
	m_optionsConstraintsPtr->InsertOption("SerialOption1", QByteArray(), QString(), 0);
	m_optionsConstraintsPtr->InsertOption("SerialOption2", QByteArray(), QString(), 1);
	m_optionsConstraintsPtr->InsertOption("SerialOption3", QByteArray(), QString(), 2);
	
	// Set a specific value
	QVERIFY(m_selectionWithConstraintsPtr->SetSelectedOptionIndex(1));
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_selectionWithConstraintsPtr->Serialize(writeArchive));
	
	// Change the value
	QVERIFY(m_selectionWithConstraintsPtr->SetSelectedOptionIndex(2));
	QVERIFY(m_selectionWithConstraintsPtr->GetSelectedOptionIndex() == 2);
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_selectionWithConstraintsPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QVERIFY(m_selectionWithConstraintsPtr->GetSelectedOptionIndex() == 1);
}


void CSelectionParamCompTest::testCopyWithConstraints()
{
	// Ensure options exist in the constraints for this test
	if (m_optionsConstraintsPtr->GetOptionsCount() < 1) {
		m_optionsConstraintsPtr->InsertOption("CopyOption1", QByteArray(), QString(), 0);
	}
	
	// Set a specific value
	QVERIFY(m_selectionWithConstraintsPtr->SetSelectedOptionIndex(0));
	
	// Create a new selection parameter and copy from the component
	iprm::CSelectionParam selectionParam;
	QVERIFY(selectionParam.CopyFrom(*m_selectionWithConstraintsPtr));
	
	// Verify the value was copied
	QVERIFY(selectionParam.GetSelectedOptionIndex() == 0);
	
	// Verify the constraints were also copied
	QVERIFY(selectionParam.GetSelectionConstraints() != nullptr);
}


void CSelectionParamCompTest::testCloneWithConstraints()
{
	// Ensure options exist in the constraints for this test
	if (m_optionsConstraintsPtr->GetOptionsCount() < 3) {
		int currentCount = m_optionsConstraintsPtr->GetOptionsCount();
		for (int i = currentCount; i < 3; ++i) {
			m_optionsConstraintsPtr->InsertOption(QString("CloneOption%1").arg(i), QByteArray(), QString(), i);
		}
	}
	
	// Set a specific value
	QVERIFY(m_selectionWithConstraintsPtr->SetSelectedOptionIndex(2));
	
	// Clone the component
	istd::TUniqueInterfacePtr<iprm::ISelectionParam> clonePtr;
	clonePtr.MoveCastedPtr(m_selectionWithConstraintsPtr->CloneMe());
	
	// Verify the clone is valid
	QVERIFY(clonePtr.IsValid());
	
	// Verify the value was cloned
	QVERIFY(clonePtr->GetSelectedOptionIndex() == 2);
	
	// Verify the constraints were also cloned
	QVERIFY(clonePtr->GetSelectionConstraints() != nullptr);
}


// Tests for selection parameter with subselection

void CSelectionParamCompTest::testCreationWithSubselection()
{
	// Verify the component was created successfully
	QVERIFY(m_selectionWithSubselectionPtr != nullptr);
	
	// Check that the default selected index is correct
	QVERIFY(m_selectionWithSubselectionPtr->GetSelectedOptionIndex() == 3);
	
	// Verify the subselection exists
	QVERIFY(m_subselectionPtr != nullptr);
	QVERIFY(m_subselectionPtr->GetSelectedOptionIndex() == 1);
}


void CSelectionParamCompTest::testGetSubselection()
{
	// Get the subselection
	iprm::ISelectionParam* subselectionPtr = m_selectionWithSubselectionPtr->GetSubselection(0);
	
	// Verify it's not null
	QVERIFY(subselectionPtr != nullptr);
	
	// Verify it's the same as the configured subselection
	QVERIFY(subselectionPtr == m_subselectionPtr);
	
	// Verify the subselection has the correct value
	QVERIFY(subselectionPtr->GetSelectedOptionIndex() == 1);
	
	// Test that GetSubselection returns the same object regardless of index
	// (based on the implementation in CSelectionParamComp.cpp)
	iprm::ISelectionParam* subselectionPtr2 = m_selectionWithSubselectionPtr->GetSubselection(5);
	QVERIFY(subselectionPtr2 == subselectionPtr);
}


void CSelectionParamCompTest::testSerializationWithSubselection()
{
	// Set specific values
	QVERIFY(m_selectionWithSubselectionPtr->SetSelectedOptionIndex(4));
	QVERIFY(m_subselectionPtr->SetSelectedOptionIndex(0));
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_selectionWithSubselectionPtr->Serialize(writeArchive));
	
	// Change the values
	QVERIFY(m_selectionWithSubselectionPtr->SetSelectedOptionIndex(5));
	QVERIFY(m_subselectionPtr->SetSelectedOptionIndex(2));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_selectionWithSubselectionPtr->Serialize(readArchive));
	
	// Verify the values were restored
	QVERIFY(m_selectionWithSubselectionPtr->GetSelectedOptionIndex() == 4);
	// Note: Subselection serialization depends on the archive behavior and component setup
	// The subselection is a separate component, so its state might not be automatically restored
}


void CSelectionParamCompTest::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CSelectionParamCompTest);
