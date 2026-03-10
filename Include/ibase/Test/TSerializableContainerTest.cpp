// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/TSerializableContainerTest.h>


// ACF includes
#include <ibase/TSerializableContainer.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// Test container implementation for int
class CIntSerializableContainer : public ibase::TSerializableContainer<int>
{
protected:
	// Serialize a single item
	virtual bool SerializeItem(int& item, iser::IArchive& archive, iser::CArchiveTag* /*parentTagPtr*/) override
	{
		return archive.Process(item);
	}
};


// Test container implementation for QString
class CStringSerializableContainer : public ibase::TSerializableContainer<QString>
{
protected:
	// Serialize a single item
	virtual bool SerializeItem(QString& item, iser::IArchive& archive, iser::CArchiveTag* /*parentTagPtr*/) override
	{
		return archive.Process(item);
	}
};


void TSerializableContainerTest::initTestCase()
{
	// Setup
}


void TSerializableContainerTest::cleanupTestCase()
{
	// Cleanup
}


void TSerializableContainerTest::testSerializeInt()
{
	CIntSerializableContainer container1;
	container1.PushBack(10);
	container1.PushBack(20);
	container1.PushBack(30);
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive;
	bool result = container1.Serialize(writeArchive);
	QVERIFY(result);
	
	// Deserialize from memory
	CIntSerializableContainer container2;
	iser::CMemoryReadArchive readArchive(writeArchive);
	result = container2.Serialize(readArchive);
	QVERIFY(result);
	
	// Verify contents
	QCOMPARE(container2.GetItemsCount(), 3);
	QCOMPARE(container2.GetAt(0), 10);
	QCOMPARE(container2.GetAt(1), 20);
	QCOMPARE(container2.GetAt(2), 30);
}


void TSerializableContainerTest::testSerializeString()
{
	CStringSerializableContainer container1;
	container1.PushBack("First");
	container1.PushBack("Second");
	container1.PushBack("Third");
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive;
	bool result = container1.Serialize(writeArchive);
	QVERIFY(result);
	
	// Deserialize from memory
	CStringSerializableContainer container2;
	iser::CMemoryReadArchive readArchive(writeArchive);
	result = container2.Serialize(readArchive);
	QVERIFY(result);
	
	// Verify contents
	QCOMPARE(container2.GetItemsCount(), 3);
	QCOMPARE(container2.GetAt(0), QString("First"));
	QCOMPARE(container2.GetAt(1), QString("Second"));
	QCOMPARE(container2.GetAt(2), QString("Third"));
}


void TSerializableContainerTest::testSerializeEmpty()
{
	CIntSerializableContainer container1;
	
	// Serialize empty container to memory
	iser::CMemoryWriteArchive writeArchive;
	bool result = container1.Serialize(writeArchive);
	QVERIFY(result);
	
	// Deserialize from memory
	CIntSerializableContainer container2;
	iser::CMemoryReadArchive readArchive(writeArchive);
	result = container2.Serialize(readArchive);
	QVERIFY(result);
	
	// Verify empty container
	QCOMPARE(container2.GetItemsCount(), 0);
	QVERIFY(container2.IsEmpty());
}


I_ADD_TEST(TSerializableContainerTest);


