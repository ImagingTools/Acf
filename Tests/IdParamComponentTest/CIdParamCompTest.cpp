// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CIdParamCompTest.h"


// ACF includes
#include <iprm/CIdParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CIdParamCompTest::initTestCase()
{
	m_testInstancePtr.reset(new CIdParamComponentTest);

	m_idWithDefaultPtr = m_testInstancePtr->GetInterface<iprm::IIdParam>("IdWithDefault");
	m_idNoDefaultPtr = m_testInstancePtr->GetInterface<iprm::IIdParam>("IdNoDefault");

	QVERIFY(m_idWithDefaultPtr != nullptr);
	QVERIFY(m_idNoDefaultPtr != nullptr);
}


// Tests for ID parameter with default ID

void CIdParamCompTest::testCreationWithDefault()
{
	// Verify the component was created successfully
	QVERIFY(m_idWithDefaultPtr != nullptr);
}


void CIdParamCompTest::testGetDefaultId()
{
	// Verify the default ID is set correctly
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray("default_id_123"));
}


void CIdParamCompTest::testSetId()
{
	// Test setting a new ID
	m_idWithDefaultPtr->SetId("new_id_456");
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray("new_id_456"));
	
	// Test setting empty ID
	m_idWithDefaultPtr->SetId(QByteArray());
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray());
	
	// Test setting another ID
	m_idWithDefaultPtr->SetId("test_id");
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray("test_id"));
	
	// Restore default value for other tests
	m_idWithDefaultPtr->SetId("default_id_123");
}


void CIdParamCompTest::testBinaryId()
{
	// Test setting binary ID
	QByteArray binaryId = QByteArray::fromHex("deadbeef1234");
	m_idWithDefaultPtr->SetId(binaryId);
	QCOMPARE(m_idWithDefaultPtr->GetId(), binaryId);
	
	// Test another binary ID
	QByteArray binaryId2 = QByteArray::fromHex("cafebabe");
	m_idWithDefaultPtr->SetId(binaryId2);
	QCOMPARE(m_idWithDefaultPtr->GetId(), binaryId2);
	
	// Restore default value
	m_idWithDefaultPtr->SetId("default_id_123");
}


void CIdParamCompTest::testSerializationWithDefault()
{
	// Set a specific value
	m_idWithDefaultPtr->SetId("serialize_test_id");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_idWithDefaultPtr->Serialize(writeArchive));
	
	// Change the value
	m_idWithDefaultPtr->SetId("changed_id");
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray("changed_id"));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_idWithDefaultPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray("serialize_test_id"));
	
	// Restore default value
	m_idWithDefaultPtr->SetId("default_id_123");
}


void CIdParamCompTest::testCopyWithDefault()
{
	// Set a specific value
	m_idWithDefaultPtr->SetId("copy_test_id");
	
	// Create a new ID parameter and copy from the component
	iprm::CIdParam copyParam;
	QVERIFY(copyParam.CopyFrom(*m_idWithDefaultPtr));
	
	// Verify the value was copied
	QCOMPARE(copyParam.GetId(), m_idWithDefaultPtr->GetId());
	QCOMPARE(copyParam.GetId(), QByteArray("copy_test_id"));
	
	// Restore default value
	m_idWithDefaultPtr->SetId("default_id_123");
}


void CIdParamCompTest::testCloneWithDefault()
{
	// Set a specific value
	m_idWithDefaultPtr->SetId("clone_test_id");
	
	// Clone the parameter
	istd::TUniqueInterfacePtr<iprm::IIdParam> clonedPtr;
	QVERIFY(clonedPtr.MoveCastedPtr(m_idWithDefaultPtr->CloneMe()));
	QVERIFY(clonedPtr.IsValid());
	
	// Verify the value was cloned
	QCOMPARE(clonedPtr->GetId(), m_idWithDefaultPtr->GetId());
	QCOMPARE(clonedPtr->GetId(), QByteArray("clone_test_id"));
	
	// Modify the clone and verify original is unchanged
	clonedPtr->SetId("modified_clone");
	QCOMPARE(clonedPtr->GetId(), QByteArray("modified_clone"));
	QCOMPARE(m_idWithDefaultPtr->GetId(), QByteArray("clone_test_id"));
	
	// Restore default value
	m_idWithDefaultPtr->SetId("default_id_123");
}


// Tests for ID parameter without default

void CIdParamCompTest::testCreationNoDefault()
{
	// Verify the component was created successfully
	QVERIFY(m_idNoDefaultPtr != nullptr);
}


void CIdParamCompTest::testEmptyId()
{
	// Verify the ID is empty by default (no default ID specified)
	QCOMPARE(m_idNoDefaultPtr->GetId(), QByteArray());
	
	// Test setting a value
	m_idNoDefaultPtr->SetId("some_id");
	QCOMPARE(m_idNoDefaultPtr->GetId(), QByteArray("some_id"));
	
	// Reset to empty
	m_idNoDefaultPtr->SetId(QByteArray());
	QCOMPARE(m_idNoDefaultPtr->GetId(), QByteArray());
}


void CIdParamCompTest::testSerializationNoDefault()
{
	// Set a specific value
	m_idNoDefaultPtr->SetId("no_default_serialize");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(m_idNoDefaultPtr->Serialize(writeArchive));
	
	// Change the value
	m_idNoDefaultPtr->SetId("different_id");
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_idNoDefaultPtr->Serialize(readArchive));
	
	// Verify the value was restored
	QCOMPARE(m_idNoDefaultPtr->GetId(), QByteArray("no_default_serialize"));
	
	// Reset to empty
	m_idNoDefaultPtr->SetId(QByteArray());
}


void CIdParamCompTest::cleanupTestCase()
{
	m_idWithDefaultPtr = nullptr;
	m_idNoDefaultPtr = nullptr;
	m_testInstancePtr.reset();
}


I_ADD_TEST(CIdParamCompTest);
