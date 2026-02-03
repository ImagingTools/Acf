// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CTextParamCompTest.h"


// ACF includes
#include <iprm/CTextParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CTextParamCompTest::initTestCase()
{
	m_testInstancePtr.reset(new CTextParamComponentTest);

	m_textWithDefaultPtr = m_testInstancePtr->GetInterface<iprm::ITextParam>("TextWithDefault");
	m_textNoDefaultPtr = m_testInstancePtr->GetInterface<iprm::ITextParam>("TextNoDefault");
	m_textReadOnlyPtr = m_testInstancePtr->GetInterface<iprm::ITextParam>("TextReadOnly");
	m_textNoSerializePtr = m_testInstancePtr->GetInterface<iprm::ITextParam>("TextNoSerialize");

	QVERIFY(m_textWithDefaultPtr != nullptr);
	QVERIFY(m_textNoDefaultPtr != nullptr);
	QVERIFY(m_textReadOnlyPtr != nullptr);
	QVERIFY(m_textNoSerializePtr != nullptr);
}


// Tests for text parameter with default text

void CTextParamCompTest::testCreationWithDefault()
{
	// Verify the component was created successfully
	QVERIFY(m_textWithDefaultPtr != nullptr);
	
	// Check that the text parameter is not read-only
	QVERIFY(!m_textWithDefaultPtr->IsReadOnly());
}


void CTextParamCompTest::testGetDefaultText()
{
	// Verify the default text is set correctly
	QCOMPARE(m_textWithDefaultPtr->GetText(), QString("Default Text Value"));
}


void CTextParamCompTest::testSetText()
{
	// Test setting a new value
	m_textWithDefaultPtr->SetText("New Text Value");
	QCOMPARE(m_textWithDefaultPtr->GetText(), QString("New Text Value"));
	
	// Test setting empty text
	m_textWithDefaultPtr->SetText("");
	QCOMPARE(m_textWithDefaultPtr->GetText(), QString(""));
	
	// Test setting multiline text
	QString multilineText = "Line 1\nLine 2\nLine 3";
	m_textWithDefaultPtr->SetText(multilineText);
	QCOMPARE(m_textWithDefaultPtr->GetText(), multilineText);
	
	// Restore default value for other tests
	m_textWithDefaultPtr->SetText("Default Text Value");
}


void CTextParamCompTest::testSerializationWithDefault()
{
	// Set a specific value
	m_textWithDefaultPtr->SetText("Test Serialization");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_textWithDefaultPtr->Serialize(writeArchive));
	
	// Change the value
	m_textWithDefaultPtr->SetText("Changed Value");
	QCOMPARE(m_textWithDefaultPtr->GetText(), QString("Changed Value"));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_textWithDefaultPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QCOMPARE(m_textWithDefaultPtr->GetText(), QString("Test Serialization"));
	
	// Restore default value
	m_textWithDefaultPtr->SetText("Default Text Value");
}


void CTextParamCompTest::testCopyWithDefault()
{
	// Set a specific value
	m_textWithDefaultPtr->SetText("Copy Test Value");
	
	// Create a new text parameter and copy from the component
	iprm::CTextParam copyParam;
	QVERIFY(copyParam.CopyFrom(*m_textWithDefaultPtr));
	
	// Verify the value was copied
	QCOMPARE(copyParam.GetText(), m_textWithDefaultPtr->GetText());
	QCOMPARE(copyParam.GetText(), QString("Copy Test Value"));
	
	// Restore default value
	m_textWithDefaultPtr->SetText("Default Text Value");
}


void CTextParamCompTest::testCloneWithDefault()
{
	// Set a specific value
	m_textWithDefaultPtr->SetText("Clone Test Value");
	
	// Clone the parameter
	auto clonedPtr = m_textWithDefaultPtr->CloneMe();
	QVERIFY(clonedPtr != nullptr);
	
	auto clonedTextParam = dynamic_cast<iprm::ITextParam*>(clonedPtr.get());
	QVERIFY(clonedTextParam != nullptr);
	
	// Verify the value was cloned
	QCOMPARE(clonedTextParam->GetText(), m_textWithDefaultPtr->GetText());
	QCOMPARE(clonedTextParam->GetText(), QString("Clone Test Value"));
	
	// Modify the clone and verify original is unchanged
	clonedTextParam->SetText("Modified Clone");
	QCOMPARE(clonedTextParam->GetText(), QString("Modified Clone"));
	QCOMPARE(m_textWithDefaultPtr->GetText(), QString("Clone Test Value"));
	
	// Restore default value
	m_textWithDefaultPtr->SetText("Default Text Value");
}


// Tests for text parameter without default

void CTextParamCompTest::testCreationNoDefault()
{
	// Verify the component was created successfully
	QVERIFY(m_textNoDefaultPtr != nullptr);
	
	// Check that the text parameter is not read-only
	QVERIFY(!m_textNoDefaultPtr->IsReadOnly());
}


void CTextParamCompTest::testEmptyText()
{
	// Verify the text is empty by default (no default text specified)
	QCOMPARE(m_textNoDefaultPtr->GetText(), QString(""));
	
	// Test setting a value
	m_textNoDefaultPtr->SetText("Some Value");
	QCOMPARE(m_textNoDefaultPtr->GetText(), QString("Some Value"));
	
	// Reset to empty
	m_textNoDefaultPtr->SetText("");
	QCOMPARE(m_textNoDefaultPtr->GetText(), QString(""));
}


void CTextParamCompTest::testSerializationNoDefault()
{
	// Set a specific value
	m_textNoDefaultPtr->SetText("Serialize No Default");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_textNoDefaultPtr->Serialize(writeArchive));
	
	// Change the value
	m_textNoDefaultPtr->SetText("Different Value");
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_textNoDefaultPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QCOMPARE(m_textNoDefaultPtr->GetText(), QString("Serialize No Default"));
	
	// Reset to empty
	m_textNoDefaultPtr->SetText("");
}


// Tests for read-only text parameter

void CTextParamCompTest::testReadOnlyCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_textReadOnlyPtr != nullptr);
	
	// Check that the text parameter is read-only
	QVERIFY(m_textReadOnlyPtr->IsReadOnly());
}


void CTextParamCompTest::testReadOnlyBehavior()
{
	// Verify the default text is set
	QCOMPARE(m_textReadOnlyPtr->GetText(), QString("Read-Only Text"));
	
	// Note: The component allows setting text even when read-only
	// This is implementation behavior - the IsReadOnly() flag is informational
	// for UI or other consumers to decide whether to allow editing
	m_textReadOnlyPtr->SetText("Attempted Change");
	QCOMPARE(m_textReadOnlyPtr->GetText(), QString("Attempted Change"));
	
	// Restore original value
	m_textReadOnlyPtr->SetText("Read-Only Text");
}


void CTextParamCompTest::testReadOnlySerialization()
{
	// The read-only parameter should still be serializable
	QString originalText = m_textReadOnlyPtr->GetText();
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_textReadOnlyPtr->Serialize(writeArchive));
	
	// Change the value
	m_textReadOnlyPtr->SetText("Modified");
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_textReadOnlyPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QCOMPARE(m_textReadOnlyPtr->GetText(), originalText);
}


// Tests for serialization control

void CTextParamCompTest::testNoSerializationParam()
{
	// Set a specific value
	m_textNoSerializePtr->SetText("No Serialize Test");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	// This should still succeed, but the SerializeParams flag affects behavior
	QVERIFY(m_textNoSerializePtr->Serialize(writeArchive));
	
	// Change the value
	QString changedValue = "Changed After Serialize";
	m_textNoSerializePtr->SetText(changedValue);
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_textNoSerializePtr->Serialize(readArchive));
	
	// With SerializeParams=false, the value should not be restored
	// It should remain as the changed value
	QCOMPARE(m_textNoSerializePtr->GetText(), changedValue);
}


void CTextParamCompTest::cleanupTestCase()
{
	m_textWithDefaultPtr = nullptr;
	m_textNoDefaultPtr = nullptr;
	m_textReadOnlyPtr = nullptr;
	m_textNoSerializePtr = nullptr;
	m_testInstancePtr.reset();
}


I_ADD_TEST(CTextParamCompTest);
