// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CSignalBlockerTest.h"


// Qt includes
#include <QtCore/QSignalSpy>


// Test helper class
class TestObject : public QObject
{
	Q_OBJECT
public:
	explicit TestObject(QObject* parent = nullptr) : QObject(parent) {}

signals:
	void testSignal();

public slots:
	void emitTestSignal() { emit testSignal(); }
};


// protected slots

void CSignalBlockerTest::initTestCase()
{
}


void CSignalBlockerTest::TestBlockSingleObject()
{
	// Create a test object
	TestObject testObj;
	QSignalSpy spy(&testObj, &TestObject::testSignal);

	// Verify signal is emitted normally
	testObj.emitTestSignal();
	QCOMPARE(spy.count(), 1);

	// Block signals using CSignalBlocker
	{
		iqt::CSignalBlocker blocker(&testObj, false);
		testObj.emitTestSignal();
		// Signal should be blocked
		QCOMPARE(spy.count(), 1);
	}

	// After blocker is destroyed, signals should work again
	testObj.emitTestSignal();
	QCOMPARE(spy.count(), 2);
}


void CSignalBlockerTest::TestBlockWithChildren()
{
	// Create parent and child objects
	TestObject parent;
	TestObject child1(&parent);
	TestObject child2(&parent);

	QSignalSpy parentSpy(&parent, &TestObject::testSignal);
	QSignalSpy child1Spy(&child1, &TestObject::testSignal);
	QSignalSpy child2Spy(&child2, &TestObject::testSignal);

	// Verify signals work normally
	parent.emitTestSignal();
	child1.emitTestSignal();
	child2.emitTestSignal();
	QCOMPARE(parentSpy.count(), 1);
	QCOMPARE(child1Spy.count(), 1);
	QCOMPARE(child2Spy.count(), 1);

	// Block parent and children
	{
		iqt::CSignalBlocker blocker(&parent, true);
		parent.emitTestSignal();
		child1.emitTestSignal();
		child2.emitTestSignal();
		// All signals should be blocked
		QCOMPARE(parentSpy.count(), 1);
		QCOMPARE(child1Spy.count(), 1);
		QCOMPARE(child2Spy.count(), 1);
	}

	// After blocker is destroyed, signals should work again
	parent.emitTestSignal();
	child1.emitTestSignal();
	child2.emitTestSignal();
	QCOMPARE(parentSpy.count(), 2);
	QCOMPARE(child1Spy.count(), 2);
	QCOMPARE(child2Spy.count(), 2);
}


void CSignalBlockerTest::TestNullPointer()
{
	// Test that CSignalBlocker handles nullptr gracefully (no-op)
	{
		iqt::CSignalBlocker blocker(nullptr, false);
		// Should not crash
	}

	{
		iqt::CSignalBlocker blocker(nullptr, true);
		// Should not crash
	}

	// Test passes if no crash occurs
	QVERIFY(true);
}


void CSignalBlockerTest::TestScopeBasedBlocking()
{
	TestObject testObj;
	QSignalSpy spy(&testObj, &TestObject::testSignal);

	// Inner scope with signal blocking
	{
		iqt::CSignalBlocker blocker(&testObj, false);
		testObj.emitTestSignal();
		QCOMPARE(spy.count(), 0);
	} // Blocker destroyed here, signals restored

	// Outer scope - signals should work
	testObj.emitTestSignal();
	QCOMPARE(spy.count(), 1);
}


void CSignalBlockerTest::TestNestedBlockers()
{
	TestObject testObj;
	QSignalSpy spy(&testObj, &TestObject::testSignal);

	// First blocker
	{
		iqt::CSignalBlocker blocker1(&testObj, false);
		testObj.emitTestSignal();
		QCOMPARE(spy.count(), 0);

		// Nested blocker
		{
			iqt::CSignalBlocker blocker2(&testObj, false);
			testObj.emitTestSignal();
			QCOMPARE(spy.count(), 0);
		}

		// Inner blocker destroyed, signals still blocked by outer blocker
		testObj.emitTestSignal();
		QCOMPARE(spy.count(), 0);
	}

	// Both blockers destroyed, signals should work
	testObj.emitTestSignal();
	QCOMPARE(spy.count(), 1);
}


void CSignalBlockerTest::cleanupTestCase()
{
}


I_ADD_TEST(CSignalBlockerTest);

// Include the moc file for TestObject
#include "CSignalBlockerTest.moc"
