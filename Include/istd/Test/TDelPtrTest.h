// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/TDelPtr.h>
#include <itest/CStandardTestExecutor.h>

// Test class with destructor counter
class TestObject
{
public:
	static int destructorCount;
	int value;

	TestObject(int val = 0) : value(val) {}
	virtual ~TestObject() { destructorCount++; }
};

class TDelPtrTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void PointerConstructorTest();
	void DestructorTest();
	void SetPtrTest();
	void ResetTest();
	void PopPtrTest();
	void TakeOverTest();
	void MoveConstructorTest();
	void MoveAssignmentTest();

	void cleanupTestCase();
};


