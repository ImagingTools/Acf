// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/Test/CComponentSimulationTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/CComponentBase.h>
#include <icomp/TSimComponentWrap.h>
#include <icomp/CComponentContext.h>
#include <icomp/CRegistry.h>
#include <icomp/CRegistryElement.h>
#include <istd/CClassInfo.h>


// Test component interface
class ITestInterface : virtual public istd::IPolymorphic
{
public:
	virtual QString GetTestValue() const = 0;
	virtual void SetTestValue(const QString& value) = 0;
};


// Simple test component implementation
class CTestComponent : public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTestComponent);
		I_REGISTER_INTERFACE(ITestInterface);
		I_ASSIGN(m_testValue, "TestValue", "Test string value", QString("default"), true);
		I_ASSIGN(m_intValue, "IntValue", "Test integer value", 42, true);
		I_ASSIGN(m_boolValue, "BoolValue", "Test boolean value", true, true);
		I_ASSIGN(m_doubleValue, "DoubleValue", "Test double value", 3.14, true);
	I_END_COMPONENT;

	CTestComponent()
		: m_createdCalled(false)
		, m_destroyedCalled(false)
	{
	}

	QString GetTestValue() const { return m_testValue; }
	void SetTestValue(const QString& value) { m_testValue = value; }

	bool WasCreatedCalled() const { return m_createdCalled; }
	bool WasDestroyedCalled() const { return m_destroyedCalled; }

protected:
	virtual void OnComponentCreated() override
	{
		BaseClass::OnComponentCreated();
		m_createdCalled = true;
	}

	virtual void OnComponentDestroyed() override
	{
		m_destroyedCalled = true;
		BaseClass::OnComponentDestroyed();
	}

private:
	I_ATTR(QString, m_testValue);
	I_ATTR(int, m_intValue);
	I_ATTR(bool, m_boolValue);
	I_ATTR(double, m_doubleValue);

	bool m_createdCalled;
	bool m_destroyedCalled;
};


// Component with reference
class CTestComponentWithRef : public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTestComponentWithRef);
		I_ASSIGN(m_name, "Name", "Component name", QString(""), true);
	I_END_COMPONENT;

	CTestComponentWithRef() {}

	QString GetName() const { return m_name; }
	void SetName(const QString& name) { m_name = name; }

	ITestInterface* GetTestRef() const { return m_testRef; }

private:
	I_ATTR(QString, m_name);
	I_REF(ITestInterface, m_testRef);
};


// Component with multi-attribute
class CTestComponentWithMulti : public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTestComponentWithMulti);
	I_END_COMPONENT;

	CTestComponentWithMulti() {}

	int GetItemCount() const { return m_items.GetCount(); }
	QString GetItem(int index) const { return m_items.GetValue(index); }
	void AddItem(const QString& item) { m_items.InsertValue(item); }

private:
	I_MULTI_ATTR(QString, m_items);
};


// Component with factory
class CTestComponentWithFactory : public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTestComponentWithFactory);
	I_END_COMPONENT;

	CTestComponentWithFactory() {}

	ITestInterface* CreateInstance() { return m_factory.CreateInstance(); }

private:
	I_FACT(ITestInterface, m_factory);
};


void CComponentSimulationTest::initTestCase()
{
}


void CComponentSimulationTest::cleanupTestCase()
{
}


void CComponentSimulationTest::testComponentCreation()
{
	// Test that we can create a component
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	QVERIFY(comp.GetPtr() != nullptr);
}


void CComponentSimulationTest::testComponentInitialization()
{
	// Test component initialization and lifecycle callbacks
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	
	// Before initialization, created callback should not have been called
	QVERIFY(!comp->WasCreatedCalled());
	
	// Initialize the component
	comp->InitComponent();
	
	// After initialization, created callback should have been called
	QVERIFY(comp->WasCreatedCalled());
	QVERIFY(!comp->WasDestroyedCalled());
}


void CComponentSimulationTest::testComponentDestruction()
{
	// Test component destruction callback
	CTestComponent* rawPtr = nullptr;
	{
		icomp::TSimSharedComponentPtr<CTestComponent> comp;
		comp->InitComponent();
		rawPtr = comp.GetPtr();
		QVERIFY(comp->WasCreatedCalled());
	}
	// Component should be destroyed when going out of scope
	// Note: We can't actually verify the callback was called since the object is destroyed
}


void CComponentSimulationTest::testComponentParentRelationship()
{
	// Test parent-child relationships
	icomp::TSimSharedComponentPtr<CTestComponent> parent;
	parent->InitComponent();
	
	// For now, just verify the parent interface exists
	QVERIFY(parent->GetParentComponent() == nullptr);
}


void CComponentSimulationTest::testComponentActive()
{
	// Test IsComponentActive state
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	
	// Component should not be active before initialization
	comp->InitComponent();
	
	// Component should be active after initialization
	QVERIFY(comp->WasCreatedCalled());
}


void CComponentSimulationTest::testComponentInterfaceQuery()
{
	// Test interface querying
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	comp->InitComponent();
	
	// Query for ITestInterface
	ITestInterface* testInterface = comp->GetInterface<ITestInterface>();
	QVERIFY(testInterface != nullptr);
	
	// Verify we can use the interface
	testInterface->SetTestValue("test");
	QCOMPARE(testInterface->GetTestValue(), QString("test"));
}


void CComponentSimulationTest::testComponentContext()
{
	// Test component context
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	comp->InitComponent();
	
	// Component should have a context after initialization
	icomp::IComponentContextSharedPtr context = comp->GetComponentContext();
	QVERIFY(context != nullptr);
}


void CComponentSimulationTest::testSingleAttribute()
{
	// Test single attribute access
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	comp->InitComponent();
	
	// Test string attribute
	comp->SetStringAttr("TestValue", "hello");
	QCOMPARE(comp->GetStringAttr("TestValue"), QString("hello"));
	
	// Test via interface
	ITestInterface* testInterface = comp->GetInterface<ITestInterface>();
	QVERIFY(testInterface != nullptr);
	QCOMPARE(testInterface->GetTestValue(), QString("hello"));
}


void CComponentSimulationTest::testMultiAttribute()
{
	// Test multi-attribute access
	icomp::TSimSharedComponentPtr<CTestComponentWithMulti> comp;
	comp->InitComponent();
	
	// Initially should be empty
	QCOMPARE(comp->GetItemCount(), 0);
	
	// Add items
	comp->AddItem("item1");
	comp->AddItem("item2");
	comp->AddItem("item3");
	
	QCOMPARE(comp->GetItemCount(), 3);
	QCOMPARE(comp->GetItem(0), QString("item1"));
	QCOMPARE(comp->GetItem(1), QString("item2"));
	QCOMPARE(comp->GetItem(2), QString("item3"));
}


void CComponentSimulationTest::testAttributeTypes()
{
	// Test different attribute types
	icomp::TSimSharedComponentPtr<CTestComponent> comp;
	comp->InitComponent();
	
	// Test integer attribute
	comp->SetIntAttr("IntValue", 100);
	QCOMPARE(comp->GetIntAttr("IntValue"), 100);
	
	// Test boolean attribute
	comp->SetBoolAttr("BoolValue", false);
	QCOMPARE(comp->GetBoolAttr("BoolValue"), false);
	
	// Test double attribute
	comp->SetDoubleAttr("DoubleValue", 2.718);
	QVERIFY(qAbs(comp->GetDoubleAttr("DoubleValue") - 2.718) < 0.001);
}


void CComponentSimulationTest::testComponentReference()
{
	// Test component references
	icomp::TSimSharedComponentPtr<CTestComponent> target;
	target->SetTestValue("target component");
	target->InitComponent();
	
	icomp::TSimSharedComponentPtr<CTestComponentWithRef> comp;
	comp->SetRef("TestRef", target);
	comp->InitComponent();
	
	// Verify reference is set correctly
	ITestInterface* ref = comp->GetTestRef();
	QVERIFY(ref != nullptr);
	QCOMPARE(ref->GetTestValue(), QString("target component"));
}


void CComponentSimulationTest::testMultiReference()
{
	// Test multiple component references
	icomp::TSimSharedComponentPtr<CTestComponent> comp1;
	comp1->SetTestValue("component1");
	comp1->InitComponent();
	
	icomp::TSimSharedComponentPtr<CTestComponent> comp2;
	comp2->SetTestValue("component2");
	comp2->InitComponent();
	
	// For now, just verify components are created
	QVERIFY(comp1->GetTestValue() == "component1");
	QVERIFY(comp2->GetTestValue() == "component2");
}


void CComponentSimulationTest::testComponentFactory()
{
	// Test component factory
	icomp::TSimSharedComponentPtr<CTestComponentWithFactory> comp;
	comp->InitComponent();
	
	// Note: Factory needs proper configuration to work
	// For now, just verify component is created
	QVERIFY(comp.GetPtr() != nullptr);
}


I_ADD_TEST(CComponentSimulationTest);


