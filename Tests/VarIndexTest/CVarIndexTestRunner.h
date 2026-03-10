// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CVarIndex.h>
#include <istd/TIndex.h>

class CVarIndexTestRunner: public QObject
{
	Q_OBJECT
private slots:
	// Constructor tests
	void testDefaultConstructor();
	void testSizeConstructor();
	void testCopyConstructor();
	void testStdVectorConstructor();
	void testQVectorConstructor();
	void testTIndexConstructor();

	// Dimension management tests
	void testGetDimensionsCount();
	void testSetDimensionsCount();
	void testIsDimensionsCountFixed();

	// Element access tests
	void testGetAt();
	void testSetAt();
	void testOperatorBracket();
	void testSetAllTo();

	// Validation tests
	void testIsValid();
	void testIsZero();
	void testIsSizeEmpty();

	// Reset and Clear tests
	void testReset();
	void testClear();

	// Iterator tests
	void testIterators();

	// Increment/Decrement tests
	void testIncreaseAt();
	void testDecreaseAt();
	void testIncrease();
	void testDecrease();

	// Boundary tests
	void testIsInside();

	// Utility methods tests
	void testGetProductVolume();
	void testGetIterationIndex();
	void testGetMinDimensionsCount();
	void testGetExpanded();
	void testToStdVector();

	// Comparison operators tests
	void testEqualityOperators();
	void testLessThanOperator();
	void testGreaterThanOperator();
	void testLessOrEqualOperator();
	void testGreaterOrEqualOperator();

	// Assignment operators tests
	void testAssignmentOperator();
	void testPlusEqualOperator();
	void testMinusEqualOperator();

	// Hash function test
	void testQHash();
};
