// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TDelPtrTest.h"


// Static member initialization
int TestObject::destructorCount = 0;


// protected slots

void TDelPtrTest::initTestCase()
{
	TestObject::destructorCount = 0;
}


void TDelPtrTest::DefaultConstructorTest()
{
	istd::TDelPtr<TestObject> ptr;
	QVERIFY(ptr.GetPtr() == nullptr);
	QVERIFY(ptr.GetPtr() == nullptr);
}


void TDelPtrTest::PointerConstructorTest()
{
	TestObject* obj = new TestObject(42);
	istd::TDelPtr<TestObject> ptr(obj);
	
	QVERIFY(ptr.GetPtr() == obj);
	QVERIFY(ptr.GetPtr() != nullptr);
	QCOMPARE(ptr->value, 42);
}


void TDelPtrTest::DestructorTest()
{
	TestObject::destructorCount = 0;
	
	{
		istd::TDelPtr<TestObject> ptr(new TestObject(10));
		QCOMPARE(TestObject::destructorCount, 0);
	}
	// Destructor should delete the object
	QCOMPARE(TestObject::destructorCount, 1);
}


void TDelPtrTest::SetPtrTest()
{
	TestObject::destructorCount = 0;
	
	istd::TDelPtr<TestObject> ptr;
	QVERIFY(ptr.GetPtr() == nullptr);
	
	// Set first object
	ptr.SetPtr(new TestObject(5));
	QVERIFY(ptr.GetPtr() != nullptr);
	QCOMPARE(ptr->value, 5);
	QCOMPARE(TestObject::destructorCount, 0);
	
	// Set second object - first should be deleted
	ptr.SetPtr(new TestObject(10));
	QCOMPARE(ptr->value, 10);
	QCOMPARE(TestObject::destructorCount, 1);
	
	// Set to null - second should be deleted
	ptr.SetPtr(nullptr);
	QVERIFY(ptr.GetPtr() == nullptr);
	QCOMPARE(TestObject::destructorCount, 2);
}


void TDelPtrTest::ResetTest()
{
	TestObject::destructorCount = 0;
	
	istd::TDelPtr<TestObject> ptr(new TestObject(20));
	QVERIFY(ptr.GetPtr() != nullptr);
	QCOMPARE(TestObject::destructorCount, 0);
	
	ptr.Reset();
	QVERIFY(ptr.GetPtr() == nullptr);
	QCOMPARE(TestObject::destructorCount, 1);
	
	// Reset on null pointer should be safe
	ptr.Reset();
	QVERIFY(ptr.GetPtr() == nullptr);
	QCOMPARE(TestObject::destructorCount, 1); // Count shouldn't change
}


void TDelPtrTest::PopPtrTest()
{
	TestObject::destructorCount = 0;
	
	TestObject* obj = new TestObject(30);
	istd::TDelPtr<TestObject> ptr(obj);
	
	QVERIFY(ptr.GetPtr() != nullptr);
	QCOMPARE(TestObject::destructorCount, 0);
	
	// Pop the pointer - should return the pointer without deleting
	TestObject* poppedPtr = ptr.PopRootPtr();
	QVERIFY(ptr.GetPtr() == nullptr);
	QVERIFY(poppedPtr == obj);
	QCOMPARE(TestObject::destructorCount, 0);
	
	// Clean up manually
	delete poppedPtr;
	QCOMPARE(TestObject::destructorCount, 1);
}


void TDelPtrTest::TakeOverTest()
{
	TestObject::destructorCount = 0;
	
	TestObject* obj1 = new TestObject(40);
	TestObject* obj2 = new TestObject(50);
	
	istd::TDelPtr<TestObject> ptr1(obj1);
	istd::TDelPtr<TestObject> ptr2(obj2);
	
	QCOMPARE(ptr1->value, 40);
	QCOMPARE(ptr2->value, 50);
	QCOMPARE(TestObject::destructorCount, 0);
	
	// ptr1 takes over ptr2's object - ptr1's old object should be deleted
	ptr1.TakeOver(ptr2);
	
	QVERIFY(ptr2.GetPtr() == nullptr);
	QCOMPARE(ptr1->value, 50);
	QCOMPARE(TestObject::destructorCount, 1); // obj1 was deleted
}


void TDelPtrTest::MoveConstructorTest()
{
	TestObject::destructorCount = 0;
	
	istd::TDelPtr<TestObject> ptr1(new TestObject(60));
	QCOMPARE(ptr1->value, 60);
	
	// Move construct ptr2 from ptr1
	istd::TDelPtr<TestObject> ptr2(std::move(ptr1));
	
	QVERIFY(ptr1.GetPtr() == nullptr);
	QVERIFY(ptr2.GetPtr() != nullptr);
	QCOMPARE(ptr2->value, 60);
	QCOMPARE(TestObject::destructorCount, 0);
}


void TDelPtrTest::MoveAssignmentTest()
{
	TestObject::destructorCount = 0;
	
	istd::TDelPtr<TestObject> ptr1(new TestObject(70));
	istd::TDelPtr<TestObject> ptr2(new TestObject(80));
	
	QCOMPARE(ptr1->value, 70);
	QCOMPARE(ptr2->value, 80);
	QCOMPARE(TestObject::destructorCount, 0);
	
	// Move assign ptr1 from ptr2 - ptr1's old object should be deleted
	ptr1 = std::move(ptr2);
	
	QVERIFY(ptr2.GetPtr() == nullptr);
	QCOMPARE(ptr1->value, 80);
	QCOMPARE(TestObject::destructorCount, 1); // First object deleted
}


void TDelPtrTest::cleanupTestCase()
{
}


I_ADD_TEST(TDelPtrTest);
