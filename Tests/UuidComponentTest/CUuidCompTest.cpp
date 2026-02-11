// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CUuidCompTest.h"


// Qt includes
#include <QtCore/QUuid>
#include <QtCore/QRegularExpression>

// ACF includes
#include <iprm/INameParam.h>
#include <iser/ISerializable.h>
#include <iser/CMemoryArchive.h>


// protected slots

void CUuidCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CUuidComponentTest);

	m_uuidCompPtr1 = m_testInstanceCompPtr->GetInterface<iprm::INameParam>("UuidComp1");
	m_uuidCompPtr2 = m_testInstanceCompPtr->GetInterface<iprm::INameParam>("UuidComp2");
	m_uuidCompPtr3 = m_testInstanceCompPtr->GetInterface<iprm::INameParam>("UuidComp3");

	QVERIFY(m_uuidCompPtr1 != nullptr);
	QVERIFY(m_uuidCompPtr2 != nullptr);
	QVERIFY(m_uuidCompPtr3 != nullptr);
}


void CUuidCompTest::cleanupTestCase()
{
	m_uuidCompPtr1 = nullptr;
	m_uuidCompPtr2 = nullptr;
	m_uuidCompPtr3 = nullptr;
	m_testInstanceCompPtr.reset();
}


// UUID Component tests

void CUuidCompTest::testUuidComponentCreation()
{
	// Verify the components were created successfully
	QVERIFY(m_uuidCompPtr1 != nullptr);
	QVERIFY(m_uuidCompPtr2 != nullptr);
	QVERIFY(m_uuidCompPtr3 != nullptr);
}


void CUuidCompTest::testUuidGeneration()
{
	// Get the generated UUIDs
	QString uuid1 = m_uuidCompPtr1->GetName();
	QString uuid2 = m_uuidCompPtr2->GetName();
	QString uuid3 = m_uuidCompPtr3->GetName();
	
	// Verify UUIDs are not empty
	QVERIFY(!uuid1.isEmpty());
	QVERIFY(!uuid2.isEmpty());
	QVERIFY(!uuid3.isEmpty());
	
	// Verify they are valid UUIDs by checking format
	QUuid qUuid1(uuid1);
	QUuid qUuid2(uuid2);
	QUuid qUuid3(uuid3);
	
	QVERIFY(!qUuid1.isNull());
	QVERIFY(!qUuid2.isNull());
	QVERIFY(!qUuid3.isNull());
}


void CUuidCompTest::testUuidUniqueness()
{
	// Get the generated UUIDs
	QString uuid1 = m_uuidCompPtr1->GetName();
	QString uuid2 = m_uuidCompPtr2->GetName();
	QString uuid3 = m_uuidCompPtr3->GetName();
	
	// Verify all three UUIDs are different from each other
	QVERIFY(uuid1 != uuid2);
	QVERIFY(uuid1 != uuid3);
	QVERIFY(uuid2 != uuid3);
}


void CUuidCompTest::testUuidFormat()
{
	// Get a generated UUID
	QString uuid = m_uuidCompPtr1->GetName();
	
	// UUID should be in format: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
	// or without braces: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
	QRegularExpression uuidPattern(
		"^\\{?[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}\\}?$"
	);
	
	QRegularExpressionMatch match = uuidPattern.match(uuid);
	QVERIFY(match.hasMatch());
}


void CUuidCompTest::testUuidSerialization()
{
	// Get the UUID component as serializable
	iser::ISerializable* serializablePtr1 = 
		m_testInstanceCompPtr->GetInterface<iser::ISerializable>("UuidComp1");
	QVERIFY(serializablePtr1 != nullptr);
	
	// Store the original UUID
	QString originalUuid = m_uuidCompPtr1->GetName();
	QVERIFY(!originalUuid.isEmpty());
	
	// Serialize the component
	iser::CMemoryArchive archiveWrite;
	archiveWrite.BeginStoring();
	QVERIFY(serializablePtr1->Serialize(archiveWrite));
	archiveWrite.EndStoring();
	
	// Create a new UUID component and restore from archive
	m_testInstanceCompPtr.reset(new CUuidComponentTest);
	m_uuidCompPtr1 = m_testInstanceCompPtr->GetInterface<iprm::INameParam>("UuidComp1");
	serializablePtr1 = m_testInstanceCompPtr->GetInterface<iser::ISerializable>("UuidComp1");
	QVERIFY(m_uuidCompPtr1 != nullptr);
	QVERIFY(serializablePtr1 != nullptr);
	
	// Deserialize
	iser::CMemoryArchive archiveRead(archiveWrite.GetData());
	archiveRead.BeginRestoring();
	QVERIFY(serializablePtr1->Serialize(archiveRead));
	archiveRead.EndRestoring();
	
	// Verify the UUID was restored correctly
	QString restoredUuid = m_uuidCompPtr1->GetName();
	QCOMPARE(restoredUuid, originalUuid);
}
