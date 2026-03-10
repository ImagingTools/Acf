// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/Test/TContainerTest.h>


// ACF includes
#include <ibase/TContainer.h>


void TContainerTest::initTestCase()
{
	// Setup
}


void TContainerTest::cleanupTestCase()
{
	// Cleanup
}


// Basic operations

void TContainerTest::testPushBack()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	QCOMPARE(container.GetItemsCount(), 3);
	QCOMPARE(container.GetAt(0), 10);
	QCOMPARE(container.GetAt(1), 20);
	QCOMPARE(container.GetAt(2), 30);
}


void TContainerTest::testPushFront()
{
	ibase::TContainer<int> container;
	
	container.PushFront(10);
	container.PushFront(20);
	container.PushFront(30);
	
	QCOMPARE(container.GetItemsCount(), 3);
	QCOMPARE(container.GetAt(0), 30);
	QCOMPARE(container.GetAt(1), 20);
	QCOMPARE(container.GetAt(2), 10);
}


void TContainerTest::testInsertAt()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(30);
	
	// Insert in the middle
	container.InsertAt(20, 1);
	
	QCOMPARE(container.GetItemsCount(), 3);
	QCOMPARE(container.GetAt(0), 10);
	QCOMPARE(container.GetAt(1), 20);
	QCOMPARE(container.GetAt(2), 30);
	
	// Insert at invalid index (should push back)
	container.InsertAt(40, 100);
	QCOMPARE(container.GetItemsCount(), 4);
	QCOMPARE(container.GetAt(3), 40);
}


void TContainerTest::testGetAt()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	QCOMPARE(container.GetAt(0), 10);
	QCOMPARE(container.GetAt(1), 20);
	QCOMPARE(container.GetAt(2), 30);
	
	// Test mutable access
	container.GetAt(1) = 25;
	QCOMPARE(container.GetAt(1), 25);
}


// Removal operations

void TContainerTest::testPopBack()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	container.PopBack();
	
	QCOMPARE(container.GetItemsCount(), 2);
	QCOMPARE(container.GetAt(0), 10);
	QCOMPARE(container.GetAt(1), 20);
}


void TContainerTest::testPopFront()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	container.PopFront();
	
	QCOMPARE(container.GetItemsCount(), 2);
	QCOMPARE(container.GetAt(0), 20);
	QCOMPARE(container.GetAt(1), 30);
}


void TContainerTest::testRemoveAt()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	container.RemoveAt(1);
	
	QCOMPARE(container.GetItemsCount(), 2);
	QCOMPARE(container.GetAt(0), 10);
	QCOMPARE(container.GetAt(1), 30);
}


void TContainerTest::testReset()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	container.Reset();
	
	QCOMPARE(container.GetItemsCount(), 0);
	QVERIFY(container.IsEmpty());
}


// Container info

void TContainerTest::testGetItemsCount()
{
	ibase::TContainer<int> container;
	
	QCOMPARE(container.GetItemsCount(), 0);
	
	container.PushBack(10);
	QCOMPARE(container.GetItemsCount(), 1);
	
	container.PushBack(20);
	QCOMPARE(container.GetItemsCount(), 2);
	
	container.PopBack();
	QCOMPARE(container.GetItemsCount(), 1);
}


void TContainerTest::testIsEmpty()
{
	ibase::TContainer<int> container;
	
	QVERIFY(container.IsEmpty());
	
	container.PushBack(10);
	QVERIFY(!container.IsEmpty());
	
	container.Reset();
	QVERIFY(container.IsEmpty());
}


void TContainerTest::testIsIndexValid()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	QVERIFY(container.IsIndexValid(0));
	QVERIFY(container.IsIndexValid(1));
	QVERIFY(container.IsIndexValid(2));
	QVERIFY(!container.IsIndexValid(-1));
	QVERIFY(!container.IsIndexValid(3));
}


// Operators

void TContainerTest::testSubscriptOperator()
{
	ibase::TContainer<int> container;
	
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	QCOMPARE(container[0], 10);
	QCOMPARE(container[1], 20);
	QCOMPARE(container[2], 30);
	
	// Test mutable access
	container[1] = 25;
	QCOMPARE(container[1], 25);
}


void TContainerTest::testAssignmentOperator()
{
	ibase::TContainer<int> container1;
	container1.PushBack(10);
	container1.PushBack(20);
	container1.PushBack(30);
	
	ibase::TContainer<int> container2;
	container2 = container1;
	
	QCOMPARE(container2.GetItemsCount(), 3);
	QCOMPARE(container2.GetAt(0), 10);
	QCOMPARE(container2.GetAt(1), 20);
	QCOMPARE(container2.GetAt(2), 30);
}


void TContainerTest::testEqualityOperators()
{
	ibase::TContainer<int> container1;
	container1.PushBack(10);
	container1.PushBack(20);
	
	ibase::TContainer<int> container2;
	container2.PushBack(10);
	container2.PushBack(20);
	
	ibase::TContainer<int> container3;
	container3.PushBack(10);
	container3.PushBack(30);
	
	QVERIFY(container1 == container2);
	QVERIFY(!(container1 != container2));
	QVERIFY(container1 != container3);
	QVERIFY(!(container1 == container3));
}


// Iterators

void TContainerTest::testIterators()
{
	ibase::TContainer<int> container;
	container.PushBack(10);
	container.PushBack(20);
	container.PushBack(30);
	
	// Test begin/end iterators
	int sum = 0;
	for (auto it = container.begin(); it != container.end(); ++it) {
		sum += *it;
	}
	QCOMPARE(sum, 60);
	
	// Test const iterators
	const ibase::TContainer<int>& constContainer = container;
	sum = 0;
	for (auto it = constContainer.begin(); it != constContainer.end(); ++it) {
		sum += *it;
	}
	QCOMPARE(sum, 60);
	
	// Test cbegin/cend
	sum = 0;
	for (auto it = container.cbegin(); it != container.cend(); ++it) {
		sum += *it;
	}
	QCOMPARE(sum, 60);
	
	// Test range-based for loop
	sum = 0;
	for (int value : container) {
		sum += value;
	}
	QCOMPARE(sum, 60);
}


// Standard container interface

void TContainerTest::testStandardPushOperations()
{
	ibase::TContainer<int> container;
	
	container.push_back(10);
	container.push_back(20);
	
	QCOMPARE(container.size(), 2);
	QCOMPARE(container[0], 10);
	QCOMPARE(container[1], 20);
	
	container.push_front(5);
	
	QCOMPARE(container.size(), 3);
	QCOMPARE(container[0], 5);
	QCOMPARE(container[1], 10);
	QCOMPARE(container[2], 20);
}


void TContainerTest::testStandardInsert()
{
	ibase::TContainer<int> container;
	
	container.push_back(10);
	container.push_back(30);
	
	// Insert at position 1
	auto it = container.insert(1, 20);
	
	QCOMPARE(container.size(), 3);
	QCOMPARE(container[0], 10);
	QCOMPARE(container[1], 20);
	QCOMPARE(container[2], 30);
	QCOMPARE(*it, 20);
	
	// Test move insert
	int value = 40;
	container.insert(3, std::move(value));
	QCOMPARE(container.size(), 4);
	QCOMPARE(container[3], 40);
}


void TContainerTest::testStandardRemove()
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	ibase::TContainer<int> container;
	
	container.push_back(10);
	container.push_back(20);
	container.push_back(30);
	
	container.remove(1);
	
	QCOMPARE(container.size(), 2);
	QCOMPARE(container[0], 10);
	QCOMPARE(container[1], 30);
#else
	QSKIP("remove() is only available in Qt6");
#endif
}


void TContainerTest::testStandardResize()
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	ibase::TContainer<int> container;
	
	container.push_back(10);
	container.push_back(20);
	
	container.resize(4);
	
	QCOMPARE(container.size(), 4);
#else
	QSKIP("resize() is only available in Qt6");
#endif
}


void TContainerTest::testStandardSize()
{
	ibase::TContainer<int> container;
	
	QCOMPARE(container.size(), 0);
	
	container.push_back(10);
	QCOMPARE(container.size(), 1);
	
	container.push_back(20);
	QCOMPARE(container.size(), 2);
}


I_ADD_TEST(TContainerTest);


