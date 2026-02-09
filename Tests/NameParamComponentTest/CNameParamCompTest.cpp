// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CNameParamCompTest.h"


// ACF includes
#include <iprm/CNameParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CNameParamCompTest::initTestCase()
{
	m_testInstancePtr.reset(new CNameParamComponentTest);

	m_nameWithDefaultPtr = m_testInstancePtr->GetInterface<iprm::INameParam>("NameWithDefault");
	m_nameNoDefaultPtr = m_testInstancePtr->GetInterface<iprm::INameParam>("NameNoDefault");
	m_nameFixedPtr = m_testInstancePtr->GetInterface<iprm::INameParam>("NameFixed");

	QVERIFY(m_nameWithDefaultPtr != nullptr);
	QVERIFY(m_nameNoDefaultPtr != nullptr);
	QVERIFY(m_nameFixedPtr != nullptr);
}


// Tests for name parameter with default name

void CNameParamCompTest::testCreationWithDefault()
{
	// Verify the component was created successfully
	QVERIFY(m_nameWithDefaultPtr != nullptr);
	
	// Check that the name parameter is not fixed
	QVERIFY(!m_nameWithDefaultPtr->IsNameFixed());
}


void CNameParamCompTest::testGetDefaultName()
{
	// Verify the default name is set correctly
	QCOMPARE(m_nameWithDefaultPtr->GetName(), QString("Default Name"));
}


void CNameParamCompTest::testSetName()
{
	// Test setting a new value
	m_nameWithDefaultPtr->SetName("New Name");
	QCOMPARE(m_nameWithDefaultPtr->GetName(), QString("New Name"));
	
	// Test setting empty name
	m_nameWithDefaultPtr->SetName("");
	QCOMPARE(m_nameWithDefaultPtr->GetName(), QString(""));
	
	// Test setting multiline name
	QString multilineName = "First Line\nSecond Line";
	m_nameWithDefaultPtr->SetName(multilineName);
	QCOMPARE(m_nameWithDefaultPtr->GetName(), multilineName);
	
	// Restore default value for other tests
	m_nameWithDefaultPtr->SetName("Default Name");
}


void CNameParamCompTest::testSerializationWithDefault()
{
	// Set a specific value
	m_nameWithDefaultPtr->SetName("Serialize Test Name");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_nameWithDefaultPtr->Serialize(writeArchive));
	
	// Change the value
	m_nameWithDefaultPtr->SetName("Changed Name");
	QCOMPARE(m_nameWithDefaultPtr->GetName(), QString("Changed Name"));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_nameWithDefaultPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QCOMPARE(m_nameWithDefaultPtr->GetName(), QString("Serialize Test Name"));
	
	// Restore default value
	m_nameWithDefaultPtr->SetName("Default Name");
}


void CNameParamCompTest::testCopyWithDefault()
{
	// Set a specific value
	m_nameWithDefaultPtr->SetName("Copy Test Name");
	
	// Create a new name parameter and copy from the component
	iprm::CNameParam copyParam;
	QVERIFY(copyParam.CopyFrom(*m_nameWithDefaultPtr));
	
	// Verify the value was copied
	QCOMPARE(copyParam.GetName(), m_nameWithDefaultPtr->GetName());
	QCOMPARE(copyParam.GetName(), QString("Copy Test Name"));
	
	// Restore default value
	m_nameWithDefaultPtr->SetName("Default Name");
}


void CNameParamCompTest::testCloneWithDefault()
{
	// Set a specific value
	m_nameWithDefaultPtr->SetName("Clone Test Name");
	
	// Clone the parameter
	istd::TUniqueInterfacePtr<iprm::INameParam> clonedPtr;
	QVERIFY(clonedPtr.MoveCastedPtr(m_nameWithDefaultPtr->CloneMe()));
	QVERIFY(clonedPtr.IsValid());
	
	// Verify the value was cloned
	QCOMPARE(clonedPtr->GetName(), m_nameWithDefaultPtr->GetName());
	QCOMPARE(clonedPtr->GetName(), QString("Clone Test Name"));
	
	// Modify the clone and verify original is unchanged
	clonedPtr->SetName("Modified Clone");
	QCOMPARE(clonedPtr->GetName(), QString("Modified Clone"));
	QCOMPARE(m_nameWithDefaultPtr->GetName(), QString("Clone Test Name"));
	
	// Restore default value
	m_nameWithDefaultPtr->SetName("Default Name");
}


// Tests for name parameter without default

void CNameParamCompTest::testCreationNoDefault()
{
	// Verify the component was created successfully
	QVERIFY(m_nameNoDefaultPtr != nullptr);
	
	// Check that the name parameter is not fixed
	QVERIFY(!m_nameNoDefaultPtr->IsNameFixed());
}


void CNameParamCompTest::testEmptyName()
{
	// Verify the name is empty by default (no default name specified)
	QCOMPARE(m_nameNoDefaultPtr->GetName(), QString(""));
	
	// Test setting a value
	m_nameNoDefaultPtr->SetName("Some Name");
	QCOMPARE(m_nameNoDefaultPtr->GetName(), QString("Some Name"));
	
	// Reset to empty
	m_nameNoDefaultPtr->SetName("");
	QCOMPARE(m_nameNoDefaultPtr->GetName(), QString(""));
}


// Tests for fixed name parameter

void CNameParamCompTest::testFixedNameCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_nameFixedPtr != nullptr);
	
	// Check that the name parameter is fixed
	QVERIFY(m_nameFixedPtr->IsNameFixed());
}


void CNameParamCompTest::testFixedNameBehavior()
{
	// Verify the default name is set
	QCOMPARE(m_nameFixedPtr->GetName(), QString("Fixed Name"));
	
	// When IsNameFixed() is true, SetName() should not change the name
	// The CNameParamComp::SetName() implementation checks m_isNameFixedAttrPtr
	// and only calls BaseClass2::SetName() if the flag is false
	m_nameFixedPtr->SetName("Attempted Change");
	QCOMPARE(m_nameFixedPtr->GetName(), QString("Fixed Name"));
}


void CNameParamCompTest::cleanupTestCase()
{
	m_nameWithDefaultPtr = nullptr;
	m_nameNoDefaultPtr = nullptr;
	m_nameFixedPtr = nullptr;
	m_testInstancePtr.reset();
}


I_ADD_TEST(CNameParamCompTest);
