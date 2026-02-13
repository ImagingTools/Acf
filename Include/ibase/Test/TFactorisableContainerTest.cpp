// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/TFactorisableContainerTest.h>


// ACF includes
#include <ibase/TFactorisableContainer.h>
#include <istd/TIFactory.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// Test interface
class ITestElement : public istd::IPolymorphic
{
public:
	virtual ~ITestElement() {}
	virtual int GetValue() const = 0;
	virtual void SetValue(int value) = 0;
};


// Test element implementation
class CTestElement : public ITestElement, public iser::ISerializable
{
public:
	CTestElement() : m_value(0) {}
	
	virtual int GetValue() const override { return m_value; }
	virtual void SetValue(int value) override { m_value = value; }
	
	// ISerializable implementation
	virtual bool Serialize(iser::IArchive& archive) override
	{
		return archive.Process(m_value);
	}
	
private:
	int m_value;
};


// Factory for test elements
class CTestElementFactory : public istd::TIFactory<ITestElement>
{
public:
	CTestElementFactory()
	{
		m_keys.insert("TestElement");
	}
	
	// reimplemented (istd::TIFactory)
	virtual istd::TUniqueInterfacePtr<ITestElement> CreateInstance(const QByteArray& keyId = "") const override
	{
		if (keyId == "TestElement") {
			return istd::TUniqueInterfacePtr<ITestElement>(new CTestElement());
		}
		return istd::TUniqueInterfacePtr<ITestElement>();
	}
	
	// reimplemented (istd::IFactoryInfo)
	virtual KeyList GetFactoryKeys() const override
	{
		return m_keys;
	}
	
private:
	KeyList m_keys;
};


void TFactorisableContainerTest::initTestCase()
{
	// Setup
}


void TFactorisableContainerTest::cleanupTestCase()
{
	// Cleanup
}


void TFactorisableContainerTest::testAddElement()
{
	ibase::TFactorisableContainer<ITestElement> container;
	CTestElementFactory factory;
	container.RegisterItemFactory(&factory);
	
	// Add elements
	ITestElement* elem1 = container.AddElement("TestElement");
	QVERIFY(elem1 != nullptr);
	elem1->SetValue(10);
	
	ITestElement* elem2 = container.AddElement("TestElement");
	QVERIFY(elem2 != nullptr);
	elem2->SetValue(20);
	
	// Verify container state
	QCOMPARE(container.GetItemsCount(), 2);
	QCOMPARE(container.GetElement(0)->GetValue(), 10);
	QCOMPARE(container.GetElement(1)->GetValue(), 20);
}


void TFactorisableContainerTest::testInsertElement()
{
	ibase::TFactorisableContainer<ITestElement> container;
	CTestElementFactory factory;
	container.RegisterItemFactory(&factory);
	
	// Add initial elements
	ITestElement* elem1 = container.AddElement("TestElement");
	elem1->SetValue(10);
	
	ITestElement* elem3 = container.AddElement("TestElement");
	elem3->SetValue(30);
	
	// Insert element in the middle
	ITestElement* elem2 = container.InsertElement(1, "TestElement");
	QVERIFY(elem2 != nullptr);
	elem2->SetValue(20);
	
	// Verify order
	QCOMPARE(container.GetItemsCount(), 3);
	QCOMPARE(container.GetElement(0)->GetValue(), 10);
	QCOMPARE(container.GetElement(1)->GetValue(), 20);
	QCOMPARE(container.GetElement(2)->GetValue(), 30);
}


void TFactorisableContainerTest::testGetElement()
{
	ibase::TFactorisableContainer<ITestElement> container;
	CTestElementFactory factory;
	container.RegisterItemFactory(&factory);
	
	// Add elements
	ITestElement* elem1 = container.AddElement("TestElement");
	elem1->SetValue(10);
	
	ITestElement* elem2 = container.AddElement("TestElement");
	elem2->SetValue(20);
	
	// Get elements
	QCOMPARE(container.GetElement(0)->GetValue(), 10);
	QCOMPARE(container.GetElement(1)->GetValue(), 20);
	QVERIFY(container.GetElement(-1) == nullptr);
	QVERIFY(container.GetElement(2) == nullptr);
}


void TFactorisableContainerTest::testGetElementIndex()
{
	ibase::TFactorisableContainer<ITestElement> container;
	CTestElementFactory factory;
	container.RegisterItemFactory(&factory);
	
	// Add elements
	ITestElement* elem1 = container.AddElement("TestElement");
	ITestElement* elem2 = container.AddElement("TestElement");
	ITestElement* elem3 = container.AddElement("TestElement");
	
	// Get indices
	QCOMPARE(container.GetElementIndex(*elem1), 0);
	QCOMPARE(container.GetElementIndex(*elem2), 1);
	QCOMPARE(container.GetElementIndex(*elem3), 2);
}


void TFactorisableContainerTest::testGetElementKey()
{
	ibase::TFactorisableContainer<ITestElement> container;
	CTestElementFactory factory;
	container.RegisterItemFactory(&factory);
	
	// Add elements with specific keys
	container.AddElement("TestElement");
	container.AddElement("TestElement");
	
	// Get keys
	QCOMPARE(container.GetElementKey(0), QByteArray("TestElement"));
	QCOMPARE(container.GetElementKey(1), QByteArray("TestElement"));
	QCOMPARE(container.GetElementKey(-1), QByteArray());
	QCOMPARE(container.GetElementKey(2), QByteArray());
}


void TFactorisableContainerTest::testSerialize()
{
	ibase::TFactorisableContainer<ITestElement> container1;
	CTestElementFactory factory;
	container1.RegisterItemFactory(&factory);
	
	// Add elements with values
	ITestElement* elem1 = container1.AddElement("TestElement");
	elem1->SetValue(10);
	
	ITestElement* elem2 = container1.AddElement("TestElement");
	elem2->SetValue(20);
	
	ITestElement* elem3 = container1.AddElement("TestElement");
	elem3->SetValue(30);
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive;
	bool result = container1.Serialize(writeArchive);
	QVERIFY(result);
	
	// Deserialize from memory
	ibase::TFactorisableContainer<ITestElement> container2;
	container2.RegisterItemFactory(&factory);
	iser::CMemoryReadArchive readArchive(writeArchive);
	result = container2.Serialize(readArchive);
	QVERIFY(result);
	
	// Verify contents
	QCOMPARE(container2.GetItemsCount(), 3);
	QCOMPARE(container2.GetElement(0)->GetValue(), 10);
	QCOMPARE(container2.GetElement(1)->GetValue(), 20);
	QCOMPARE(container2.GetElement(2)->GetValue(), 30);
	QCOMPARE(container2.GetElementKey(0), QByteArray("TestElement"));
	QCOMPARE(container2.GetElementKey(1), QByteArray("TestElement"));
	QCOMPARE(container2.GetElementKey(2), QByteArray("TestElement"));
}


void TFactorisableContainerTest::testSerializeEmpty()
{
	ibase::TFactorisableContainer<ITestElement> container1;
	CTestElementFactory factory;
	container1.RegisterItemFactory(&factory);
	
	// Serialize empty container to memory
	iser::CMemoryWriteArchive writeArchive;
	bool result = container1.Serialize(writeArchive);
	QVERIFY(result);
	
	// Deserialize from memory
	ibase::TFactorisableContainer<ITestElement> container2;
	container2.RegisterItemFactory(&factory);
	iser::CMemoryReadArchive readArchive(writeArchive);
	result = container2.Serialize(readArchive);
	QVERIFY(result);
	
	// Verify empty container
	QCOMPARE(container2.GetItemsCount(), 0);
	QVERIFY(container2.IsEmpty());
}


I_ADD_TEST(TFactorisableContainerTest);


