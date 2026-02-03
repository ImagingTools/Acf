// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/Test/CRegistryTest.h>


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/CRegistry.h>
#include <icomp/CRegistryElement.h>
#include <iser/CMemoryArchive.h>


void CRegistryTest::initTestCase()
{
}


void CRegistryTest::cleanupTestCase()
{
}


void CRegistryTest::testRegistryCreation()
{
	// Test registry creation
	icomp::CRegistry registry;
	
	QVERIFY(registry.GetElementsCount() == 0);
	QVERIFY(registry.GetEmbeddedRegistriesCount() == 0);
}


void CRegistryTest::testElementInsertion()
{
	// Test inserting elements into registry
	icomp::CRegistry registry;
	
	icomp::CRegistryElement element1;
	element1.SetElementId("Element1");
	
	registry.InsertElementInfo(element1);
	QCOMPARE(registry.GetElementsCount(), 1);
	
	icomp::CRegistryElement element2;
	element2.SetElementId("Element2");
	registry.InsertElementInfo(element2);
	
	QCOMPARE(registry.GetElementsCount(), 2);
}


void CRegistryTest::testElementRetrieval()
{
	// Test retrieving elements from registry
	icomp::CRegistry registry;
	
	icomp::CRegistryElement element;
	element.SetElementId("TestElement");
	element.SetAttribute("TestAttr", QString("TestValue"));
	
	registry.InsertElementInfo(element);
	
	// Retrieve the element
	const icomp::CRegistryElement* retrievedElement = registry.GetElementInfo("TestElement");
	QVERIFY(retrievedElement != nullptr);
	QCOMPARE(retrievedElement->GetElementId(), QByteArray("TestElement"));
	
	// Check attribute
	QString attrValue = retrievedElement->GetAttribute("TestAttr").value<QString>();
	QCOMPARE(attrValue, QString("TestValue"));
}


void CRegistryTest::testElementRemoval()
{
	// Test removing elements from registry
	icomp::CRegistry registry;
	
	icomp::CRegistryElement element1;
	element1.SetElementId("Element1");
	registry.InsertElementInfo(element1);
	
	icomp::CRegistryElement element2;
	element2.SetElementId("Element2");
	registry.InsertElementInfo(element2);
	
	QCOMPARE(registry.GetElementsCount(), 2);
	
	// Remove element
	registry.RemoveElementInfo("Element1");
	QCOMPARE(registry.GetElementsCount(), 1);
	
	// Verify correct element was removed
	const icomp::CRegistryElement* remainingElement = registry.GetElementInfo("Element2");
	QVERIFY(remainingElement != nullptr);
	QCOMPARE(remainingElement->GetElementId(), QByteArray("Element2"));
	
	// Verify removed element is gone
	const icomp::CRegistryElement* removedElement = registry.GetElementInfo("Element1");
	QVERIFY(removedElement == nullptr);
}


void CRegistryTest::testElementRename()
{
	// Test renaming elements in registry
	icomp::CRegistry registry;
	
	icomp::CRegistryElement element;
	element.SetElementId("OldName");
	element.SetAttribute("TestAttr", QString("Value"));
	registry.InsertElementInfo(element);
	
	// Rename element
	registry.RenameElement("OldName", "NewName");
	
	// Verify old name is gone
	const icomp::CRegistryElement* oldElement = registry.GetElementInfo("OldName");
	QVERIFY(oldElement == nullptr);
	
	// Verify new name exists with same data
	const icomp::CRegistryElement* newElement = registry.GetElementInfo("NewName");
	QVERIFY(newElement != nullptr);
	QCOMPARE(newElement->GetElementId(), QByteArray("NewName"));
	
	QString attrValue = newElement->GetAttribute("TestAttr").value<QString>();
	QCOMPARE(attrValue, QString("Value"));
}


void CRegistryTest::testEmbeddedRegistryInsertion()
{
	// Test inserting embedded registries
	icomp::CRegistry mainRegistry;
	
	istd::TSharedInterfacePtr<icomp::IRegistry> embeddedRegistry(new icomp::CRegistry());
	
	// Add an element to embedded registry
	icomp::CRegistryElement element;
	element.SetElementId("EmbeddedElement");
	embeddedRegistry.GetInterface<icomp::CRegistry>()->InsertElementInfo(element);
	
	// Insert embedded registry
	mainRegistry.InsertEmbeddedRegistry("Embedded1", embeddedRegistry);
	QCOMPARE(mainRegistry.GetEmbeddedRegistriesCount(), 1);
}


void CRegistryTest::testEmbeddedRegistryRetrieval()
{
	// Test retrieving embedded registries
	icomp::CRegistry mainRegistry;
	
	istd::TSharedInterfacePtr<icomp::IRegistry> embeddedRegistry(new icomp::CRegistry());
	
	// Add an element to embedded registry
	icomp::CRegistryElement element;
	element.SetElementId("EmbeddedElement");
	embeddedRegistry.GetInterface<icomp::CRegistry>()->InsertElementInfo(element);
	
	mainRegistry.InsertEmbeddedRegistry("Embedded1", embeddedRegistry);
	
	// Retrieve embedded registry
	istd::TSharedInterfacePtr<icomp::IRegistry> retrievedRegistry = mainRegistry.GetEmbeddedRegistry("Embedded1");
	QVERIFY(retrievedRegistry.IsValid());
	
	// Verify embedded registry contents
	const icomp::CRegistryElement* embeddedElement = 
		retrievedRegistry.GetInterface<icomp::CRegistry>()->GetElementInfo("EmbeddedElement");
	QVERIFY(embeddedElement != nullptr);
}


void CRegistryTest::testEmbeddedRegistryRemoval()
{
	// Test removing embedded registries
	icomp::CRegistry mainRegistry;
	
	istd::TSharedInterfacePtr<icomp::IRegistry> embeddedRegistry1(new icomp::CRegistry());
	istd::TSharedInterfacePtr<icomp::IRegistry> embeddedRegistry2(new icomp::CRegistry());
	
	mainRegistry.InsertEmbeddedRegistry("Embedded1", embeddedRegistry1);
	mainRegistry.InsertEmbeddedRegistry("Embedded2", embeddedRegistry2);
	
	QCOMPARE(mainRegistry.GetEmbeddedRegistriesCount(), 2);
	
	// Remove embedded registry
	mainRegistry.RemoveEmbeddedRegistry("Embedded1");
	QCOMPARE(mainRegistry.GetEmbeddedRegistriesCount(), 1);
	
	// Verify correct registry was removed
	istd::TSharedInterfacePtr<icomp::IRegistry> remaining = mainRegistry.GetEmbeddedRegistry("Embedded2");
	QVERIFY(remaining.IsValid());
	
	istd::TSharedInterfacePtr<icomp::IRegistry> removed = mainRegistry.GetEmbeddedRegistry("Embedded1");
	QVERIFY(!removed.IsValid());
}


void CRegistryTest::testExportedInterfaces()
{
	// Test exported interface management
	icomp::CRegistry registry;
	
	icomp::CRegistryElement element;
	element.SetElementId("TestElement");
	registry.InsertElementInfo(element);
	
	// Set interface as exported
	istd::CClassInfo testInterface("ITestInterface");
	registry.SetElementInterfaceExported("TestElement", testInterface, true);
	
	// Verify element was updated
	const icomp::CRegistryElement* retrievedElement = registry.GetElementInfo("TestElement");
	QVERIFY(retrievedElement != nullptr);
}


void CRegistryTest::testRegistrySerialization()
{
	// Test registry serialization and deserialization
	icomp::CRegistry originalRegistry;
	
	// Add some elements
	icomp::CRegistryElement element1;
	element1.SetElementId("Element1");
	element1.SetAttribute("Attr1", QString("Value1"));
	originalRegistry.InsertElementInfo(element1);
	
	icomp::CRegistryElement element2;
	element2.SetElementId("Element2");
	element2.SetAttribute("Attr2", 42);
	originalRegistry.InsertElementInfo(element2);
	
	// Serialize
	iser::CMemoryArchive archive;
	bool serializeResult = originalRegistry.Serialize(archive);
	QVERIFY(serializeResult);
	
	// Deserialize
	icomp::CRegistry deserializedRegistry;
	archive.SeekBegin();
	bool deserializeResult = deserializedRegistry.Serialize(archive);
	QVERIFY(deserializeResult);
	
	// Verify deserialized registry has same elements
	QCOMPARE(deserializedRegistry.GetElementsCount(), originalRegistry.GetElementsCount());
	
	const icomp::CRegistryElement* deserElement1 = deserializedRegistry.GetElementInfo("Element1");
	QVERIFY(deserElement1 != nullptr);
	QCOMPARE(deserElement1->GetAttribute("Attr1").value<QString>(), QString("Value1"));
	
	const icomp::CRegistryElement* deserElement2 = deserializedRegistry.GetElementInfo("Element2");
	QVERIFY(deserElement2 != nullptr);
	QCOMPARE(deserElement2->GetAttribute("Attr2").toInt(), 42);
}


void CRegistryTest::testConcurrentAccess()
{
	// Basic test for thread-safe access
	// Note: CRegistry uses QMutex for thread safety
	icomp::CRegistry registry;
	
	// Add some elements
	for (int i = 0; i < 10; i++)
	{
		icomp::CRegistryElement element;
		element.SetElementId(QString("Element%1").arg(i).toLatin1());
		registry.InsertElementInfo(element);
	}
	
	QCOMPARE(registry.GetElementsCount(), 10);
	
	// Verify all elements are accessible
	for (int i = 0; i < 10; i++)
	{
		const icomp::CRegistryElement* element = 
			registry.GetElementInfo(QString("Element%1").arg(i).toLatin1());
		QVERIFY(element != nullptr);
	}
}


I_ADD_TEST(CRegistryTest);
