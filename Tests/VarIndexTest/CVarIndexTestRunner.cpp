// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CVarIndexTestRunner.h"

#include <itest/CStandardTestExecutor.h>

// Constructor tests

void CVarIndexTestRunner::testDefaultConstructor()
{
	istd::CVarIndex index;
	QCOMPARE(index.GetDimensionsCount(), 0);
}

void CVarIndexTestRunner::testSizeConstructor()
{
	// Test with size 3 and default value 0
	istd::CVarIndex index1(3);
	QCOMPARE(index1.GetDimensionsCount(), 3);
	QCOMPARE(index1[0], 0);
	QCOMPARE(index1[1], 0);
	QCOMPARE(index1[2], 0);

	// Test with size 3 and value 5
	istd::CVarIndex index2(3, 5);
	QCOMPARE(index2.GetDimensionsCount(), 3);
	QCOMPARE(index2[0], 5);
	QCOMPARE(index2[1], 5);
	QCOMPARE(index2[2], 5);

	// Test with size 0
	istd::CVarIndex index3(0);
	QCOMPARE(index3.GetDimensionsCount(), 0);
}

void CVarIndexTestRunner::testCopyConstructor()
{
	istd::CVarIndex original(3, 7);
	istd::CVarIndex copy(original);
	
	QCOMPARE(copy.GetDimensionsCount(), 3);
	QCOMPARE(copy[0], 7);
	QCOMPARE(copy[1], 7);
	QCOMPARE(copy[2], 7);
	QVERIFY(copy == original);
}

void CVarIndexTestRunner::testStdVectorConstructor()
{
	std::vector<int> vec = {1, 2, 3, 4};
	istd::CVarIndex index(vec);
	
	QCOMPARE(index.GetDimensionsCount(), 4);
	QCOMPARE(index[0], 1);
	QCOMPARE(index[1], 2);
	QCOMPARE(index[2], 3);
	QCOMPARE(index[3], 4);
}

void CVarIndexTestRunner::testQVectorConstructor()
{
	QVector<int> vec;
	vec << 5 << 6 << 7;
	istd::CVarIndex index(vec);
	
	QCOMPARE(index.GetDimensionsCount(), 3);
	QCOMPARE(index[0], 5);
	QCOMPARE(index[1], 6);
	QCOMPARE(index[2], 7);
}

void CVarIndexTestRunner::testTIndexConstructor()
{
	istd::TIndex<3> fixedIndex;
	fixedIndex[0] = 10;
	fixedIndex[1] = 20;
	fixedIndex[2] = 30;
	
	istd::CVarIndex varIndex(fixedIndex);
	
	QCOMPARE(varIndex.GetDimensionsCount(), 3);
	QCOMPARE(varIndex[0], 10);
	QCOMPARE(varIndex[1], 20);
	QCOMPARE(varIndex[2], 30);
}

// Dimension management tests

void CVarIndexTestRunner::testGetDimensionsCount()
{
	istd::CVarIndex index1;
	QCOMPARE(index1.GetDimensionsCount(), 0);
	
	istd::CVarIndex index2(5);
	QCOMPARE(index2.GetDimensionsCount(), 5);
}

void CVarIndexTestRunner::testSetDimensionsCount()
{
	istd::CVarIndex index;
	
	// Set dimensions to 3
	bool result = index.SetDimensionsCount(3);
	QVERIFY(result);
	QCOMPARE(index.GetDimensionsCount(), 3);
	
	// Verify default values are 0
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
	
	// Set dimensions to 5 with initial value 7
	result = index.SetDimensionsCount(5, 7);
	QVERIFY(result);
	QCOMPARE(index.GetDimensionsCount(), 5);
	
	// First 3 elements should remain 0
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
	
	// New elements should be 7
	QCOMPARE(index[3], 7);
	QCOMPARE(index[4], 7);
	
	// Shrink dimensions
	result = index.SetDimensionsCount(2);
	QVERIFY(result);
	QCOMPARE(index.GetDimensionsCount(), 2);
}

void CVarIndexTestRunner::testIsDimensionsCountFixed()
{
	istd::CVarIndex index;
	// Variable index should always return false
	QVERIFY(!index.IsDimensionsCountFixed());
	
	index.SetDimensionsCount(5);
	QVERIFY(!index.IsDimensionsCountFixed());
}

// Element access tests

void CVarIndexTestRunner::testGetAt()
{
	istd::CVarIndex index(3);
	index[0] = 10;
	index[1] = 20;
	index[2] = 30;
	
	QCOMPARE(index.GetAt(0), 10);
	QCOMPARE(index.GetAt(1), 20);
	QCOMPARE(index.GetAt(2), 30);
}

void CVarIndexTestRunner::testSetAt()
{
	istd::CVarIndex index(3);
	
	index.SetAt(0, 100);
	index.SetAt(1, 200);
	index.SetAt(2, 300);
	
	QCOMPARE(index[0], 100);
	QCOMPARE(index[1], 200);
	QCOMPARE(index[2], 300);
}

void CVarIndexTestRunner::testOperatorBracket()
{
	istd::CVarIndex index(3);
	
	// Test write access
	index[0] = 5;
	index[1] = 15;
	index[2] = 25;
	
	// Test read access
	QCOMPARE(index[0], 5);
	QCOMPARE(index[1], 15);
	QCOMPARE(index[2], 25);
	
	// Test const access
	const istd::CVarIndex& constIndex = index;
	QCOMPARE(constIndex[0], 5);
	QCOMPARE(constIndex[1], 15);
	QCOMPARE(constIndex[2], 25);
}

void CVarIndexTestRunner::testSetAllTo()
{
	istd::CVarIndex index(4);
	
	index.SetAllTo(42);
	
	QCOMPARE(index[0], 42);
	QCOMPARE(index[1], 42);
	QCOMPARE(index[2], 42);
	QCOMPARE(index[3], 42);
}

// Validation tests

void CVarIndexTestRunner::testIsValid()
{
	// Empty index is not valid
	istd::CVarIndex index1;
	QVERIFY(!index1.IsValid());
	
	// Index with all non-negative values is valid
	istd::CVarIndex index2(3, 0);
	QVERIFY(index2.IsValid());
	
	istd::CVarIndex index3(3, 5);
	QVERIFY(index3.IsValid());
	
	// Index with negative value is not valid
	istd::CVarIndex index4(3, 0);
	index4[1] = -1;
	QVERIFY(!index4.IsValid());
}

void CVarIndexTestRunner::testIsZero()
{
	// Empty index returns true (per implementation)
	istd::CVarIndex index1;
	QVERIFY(index1.IsZero());
	
	// Index with all zeros is zero
	istd::CVarIndex index2(3, 0);
	QVERIFY(index2.IsZero());
	
	// Index with any non-zero value is not zero
	istd::CVarIndex index3(3, 0);
	index3[1] = 1;
	QVERIFY(!index3.IsZero());
	
	// Index with negative value is also not zero
	istd::CVarIndex index4(3, 0);
	index4[0] = -1;
	QVERIFY(!index4.IsZero());
}

void CVarIndexTestRunner::testIsSizeEmpty()
{
	// Empty index is empty
	istd::CVarIndex index1;
	QVERIFY(index1.IsSizeEmpty());
	
	// Index with all positive values is not empty
	istd::CVarIndex index2(3, 5);
	QVERIFY(!index2.IsSizeEmpty());
	
	// Index with zero value is empty
	istd::CVarIndex index3(3, 5);
	index3[1] = 0;
	QVERIFY(index3.IsSizeEmpty());
	
	// Index with negative value is empty
	istd::CVarIndex index4(3, 5);
	index4[2] = -1;
	QVERIFY(index4.IsSizeEmpty());
}

// Reset and Clear tests

void CVarIndexTestRunner::testReset()
{
	istd::CVarIndex index(3, 5);
	QCOMPARE(index.GetDimensionsCount(), 3);
	
	index.Reset();
	
	// After reset, dimensions should be 0
	QCOMPARE(index.GetDimensionsCount(), 0);
}

void CVarIndexTestRunner::testClear()
{
	istd::CVarIndex index(3, 5);
	QCOMPARE(index.GetDimensionsCount(), 3);
	
	index.Clear();
	
	// After clear, dimensions remain but values are 0
	QCOMPARE(index.GetDimensionsCount(), 3);
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
}

// Iterator tests

void CVarIndexTestRunner::testIterators()
{
	istd::CVarIndex index(3);
	index[0] = 10;
	index[1] = 20;
	index[2] = 30;
	
	istd::CVarIndex::Iterator begin = index.Begin();
	istd::CVarIndex::Iterator end = index.End();
	
	// Verify iterator range
	QCOMPARE(end - begin, 3);
	
	// Verify values through iterator
	QCOMPARE(*begin, 10);
	QCOMPARE(*(begin + 1), 20);
	QCOMPARE(*(begin + 2), 30);
	
	// Test iteration
	int sum = 0;
	for (istd::CVarIndex::Iterator it = begin; it != end; ++it) {
		sum += *it;
	}
	QCOMPARE(sum, 60);
}

// Increment/Decrement tests

void CVarIndexTestRunner::testIncreaseAt()
{
	istd::CVarIndex index(3, 5);
	
	bool result = index.IncreaseAt(1);
	QVERIFY(result);
	
	QCOMPARE(index[0], 5);
	QCOMPARE(index[1], 6);
	QCOMPARE(index[2], 5);
}

void CVarIndexTestRunner::testDecreaseAt()
{
	istd::CVarIndex index(3, 5);
	
	bool result = index.DecreaseAt(1);
	QVERIFY(result);
	
	QCOMPARE(index[0], 5);
	QCOMPARE(index[1], 4);
	QCOMPARE(index[2], 5);
}

void CVarIndexTestRunner::testIncrease()
{
	// Test basic increase
	istd::CVarIndex boundaries(3);
	boundaries[0] = 2;
	boundaries[1] = 3;
	boundaries[2] = 2;
	
	istd::CVarIndex index(3, 0);
	
	// First increase: [0,0,0] -> [1,0,0]
	bool result = index.Increase(boundaries);
	QVERIFY(result);
	QCOMPARE(index[0], 1);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
	
	// Second increase: [1,0,0] -> [0,1,0]
	result = index.Increase(boundaries);
	QVERIFY(result);
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 1);
	QCOMPARE(index[2], 0);
	
	// Set to near overflow
	index[0] = 1;
	index[1] = 2;
	index[2] = 1;
	
	// Should overflow and return false
	result = index.Increase(boundaries);
	QVERIFY(!result);
}

void CVarIndexTestRunner::testDecrease()
{
	// Test basic decrease
	istd::CVarIndex boundaries(3);
	boundaries[0] = 2;
	boundaries[1] = 3;
	boundaries[2] = 2;
	
	istd::CVarIndex index(3);
	index[0] = 1;
	index[1] = 1;
	index[2] = 0;
	
	// First decrease: [1,1,0] -> [0,1,0]
	bool result = index.Decrease(boundaries);
	QVERIFY(result);
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 1);
	QCOMPARE(index[2], 0);
	
	// Second decrease: [0,1,0] -> [1,0,0]
	result = index.Decrease(boundaries);
	QVERIFY(result);
	QCOMPARE(index[0], 1);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
	
	// Third decrease: [1,0,0] -> [0,0,0]
	result = index.Decrease(boundaries);
	QVERIFY(result);
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
	
	// Should underflow and return false
	result = index.Decrease(boundaries);
	QVERIFY(!result);
}

// Boundary tests

void CVarIndexTestRunner::testIsInside()
{
	istd::CVarIndex boundaries(3);
	boundaries[0] = 5;
	boundaries[1] = 10;
	boundaries[2] = 3;
	
	// Test index inside boundaries
	istd::CVarIndex index1(3);
	index1[0] = 2;
	index1[1] = 5;
	index1[2] = 1;
	QVERIFY(index1.IsInside(boundaries));
	
	// Test index at boundaries (should be outside)
	istd::CVarIndex index2(3);
	index2[0] = 5;
	index2[1] = 10;
	index2[2] = 3;
	QVERIFY(!index2.IsInside(boundaries));
	
	// Test index outside boundaries
	istd::CVarIndex index3(3);
	index3[0] = 2;
	index3[1] = 15;
	index3[2] = 1;
	QVERIFY(!index3.IsInside(boundaries));
	
	// Test with zero index
	istd::CVarIndex index4(3, 0);
	QVERIFY(index4.IsInside(boundaries));
	
	// Test with empty boundaries
	istd::CVarIndex emptyBoundaries;
	QVERIFY(!index1.IsInside(emptyBoundaries));
}

// Utility methods tests

void CVarIndexTestRunner::testGetProductVolume()
{
	// Test with size 3x4x5
	istd::CVarIndex index1(3);
	index1[0] = 3;
	index1[1] = 4;
	index1[2] = 5;
	QCOMPARE(index1.GetProductVolume(), 60);
	
	// Test with zero element
	istd::CVarIndex index2(3);
	index2[0] = 3;
	index2[1] = 0;
	index2[2] = 5;
	QCOMPARE(index2.GetProductVolume(), 0);
	
	// Test with empty index
	istd::CVarIndex index3;
	QCOMPARE(index3.GetProductVolume(), 1);
	
	// Test with single dimension
	istd::CVarIndex index4(1, 10);
	QCOMPARE(index4.GetProductVolume(), 10);
}

void CVarIndexTestRunner::testGetIterationIndex()
{
	istd::CVarIndex boundaries(3);
	boundaries[0] = 2;
	boundaries[1] = 3;
	boundaries[2] = 2;
	
	// Test [0,0,0]
	istd::CVarIndex index1(3, 0);
	QCOMPARE(index1.GetIterationIndex(boundaries), 0);
	
	// Test [1,0,0]
	istd::CVarIndex index2(3, 0);
	index2[0] = 1;
	QCOMPARE(index2.GetIterationIndex(boundaries), 1);
	
	// Test [0,1,0]
	istd::CVarIndex index3(3, 0);
	index3[1] = 1;
	QCOMPARE(index3.GetIterationIndex(boundaries), 2);
	
	// Test [1,1,0]
	istd::CVarIndex index4(3, 0);
	index4[0] = 1;
	index4[1] = 1;
	QCOMPARE(index4.GetIterationIndex(boundaries), 3);
	
	// Test [0,0,1]
	istd::CVarIndex index5(3, 0);
	index5[2] = 1;
	QCOMPARE(index5.GetIterationIndex(boundaries), 6);
	
	// Test [1,2,1]
	istd::CVarIndex index6(3);
	index6[0] = 1;
	index6[1] = 2;
	index6[2] = 1;
	QCOMPARE(index6.GetIterationIndex(boundaries), 11); // 1 + 2*2 + 1*6 = 11
}

void CVarIndexTestRunner::testGetMinDimensionsCount()
{
	// Test with all zeros
	istd::CVarIndex index1(5, 0);
	QCOMPARE(index1.GetMinDimensionsCount(), 0);
	
	// Test with last non-zero at position 2
	istd::CVarIndex index2(5, 0);
	index2[2] = 5;
	QCOMPARE(index2.GetMinDimensionsCount(), 3);
	
	// Test with last non-zero at position 4
	istd::CVarIndex index3(5, 0);
	index3[4] = 1;
	QCOMPARE(index3.GetMinDimensionsCount(), 5);
	
	// Test with mixed values
	istd::CVarIndex index4(5);
	index4[0] = 1;
	index4[1] = 2;
	index4[2] = 0;
	index4[3] = 3;
	index4[4] = 0;
	QCOMPARE(index4.GetMinDimensionsCount(), 4);
	
	// Test with empty index
	istd::CVarIndex index5;
	QCOMPARE(index5.GetMinDimensionsCount(), 0);
}

void CVarIndexTestRunner::testGetExpanded()
{
	istd::CVarIndex index1(2);
	index1[0] = 1;
	index1[1] = 2;
	
	istd::CVarIndex index2(3);
	index2[0] = 3;
	index2[1] = 4;
	index2[2] = 5;
	
	istd::CVarIndex result = index1.GetExpanded(index2);
	
	// Result should have 5 elements (2 + 3)
	QCOMPARE(result.GetDimensionsCount(), 5);
	QCOMPARE(result[0], 1);
	QCOMPARE(result[1], 2);
	QCOMPARE(result[2], 3);
	QCOMPARE(result[3], 4);
	QCOMPARE(result[4], 5);
}

void CVarIndexTestRunner::testToStdVector()
{
	istd::CVarIndex index(4);
	index[0] = 10;
	index[1] = 20;
	index[2] = 30;
	index[3] = 40;
	
	std::vector<int> vec = index.ToStdVector();
	
	QCOMPARE(vec.size(), size_t(4));
	QCOMPARE(vec[0], 10);
	QCOMPARE(vec[1], 20);
	QCOMPARE(vec[2], 30);
	QCOMPARE(vec[3], 40);
}

// Comparison operators tests

void CVarIndexTestRunner::testEqualityOperators()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 3;
	
	istd::CVarIndex index3(3);
	index3[0] = 1;
	index3[1] = 2;
	index3[2] = 4;
	
	// Test equality
	QVERIFY(index1 == index2);
	QVERIFY(!(index1 == index3));
	
	// Test inequality
	QVERIFY(!(index1 != index2));
	QVERIFY(index1 != index3);
	
	// Test with different dimensions (trailing zeros)
	istd::CVarIndex index4(4);
	index4[0] = 1;
	index4[1] = 2;
	index4[2] = 3;
	index4[3] = 0;
	
	QVERIFY(index1 == index4);
}

void CVarIndexTestRunner::testLessThanOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 4;
	
	istd::CVarIndex index3(3);
	index3[0] = 2;
	index3[1] = 2;
	index3[2] = 3;
	
	QVERIFY(index1 < index2);
	QVERIFY(index1 < index3);
	QVERIFY(!(index2 < index1));
	QVERIFY(!(index1 < index1));
	
	// Test with different dimensions
	istd::CVarIndex index4(2);
	index4[0] = 1;
	index4[1] = 2;
	
	QVERIFY(index4 < index1); // Shorter is less
}

void CVarIndexTestRunner::testGreaterThanOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 4;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 3;
	
	QVERIFY(index1 > index2);
	QVERIFY(!(index2 > index1));
	QVERIFY(!(index1 > index1));
	
	// Test with different dimensions
	istd::CVarIndex index3(4);
	index3[0] = 1;
	index3[1] = 2;
	index3[2] = 3;
	index3[3] = 0;
	
	QVERIFY(index3 > index2); // Longer is greater
}

void CVarIndexTestRunner::testLessOrEqualOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 3;
	
	istd::CVarIndex index3(3);
	index3[0] = 1;
	index3[1] = 2;
	index3[2] = 4;
	
	QVERIFY(index1 <= index2);
	QVERIFY(index1 <= index3);
	QVERIFY(!(index3 <= index1));
}

void CVarIndexTestRunner::testGreaterOrEqualOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 3;
	
	istd::CVarIndex index3(3);
	index3[0] = 1;
	index3[1] = 2;
	index3[2] = 2;
	
	QVERIFY(index1 >= index2);
	QVERIFY(index1 >= index3);
	QVERIFY(!(index3 >= index1));
}

// Assignment operators tests

void CVarIndexTestRunner::testAssignmentOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2;
	index2 = index1;
	
	QCOMPARE(index2.GetDimensionsCount(), 3);
	QCOMPARE(index2[0], 1);
	QCOMPARE(index2[1], 2);
	QCOMPARE(index2[2], 3);
	QVERIFY(index2 == index1);
}

void CVarIndexTestRunner::testPlusEqualOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2(3);
	index2[0] = 10;
	index2[1] = 20;
	index2[2] = 30;
	
	index1 += index2;
	
	QCOMPARE(index1[0], 11);
	QCOMPARE(index1[1], 22);
	QCOMPARE(index1[2], 33);
	
	// Test with different dimensions
	istd::CVarIndex index3(5);
	index3[0] = 1;
	index3[1] = 1;
	index3[2] = 1;
	index3[3] = 1;
	index3[4] = 1;
	
	istd::CVarIndex index4(2);
	index4[0] = 10;
	index4[1] = 10;
	
	index3 += index4;
	
	// Only first 2 elements should be affected
	QCOMPARE(index3[0], 11);
	QCOMPARE(index3[1], 11);
	QCOMPARE(index3[2], 1);
	QCOMPARE(index3[3], 1);
	QCOMPARE(index3[4], 1);
}

void CVarIndexTestRunner::testMinusEqualOperator()
{
	istd::CVarIndex index1(3);
	index1[0] = 10;
	index1[1] = 20;
	index1[2] = 30;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 3;
	
	index1 -= index2;
	
	QCOMPARE(index1[0], 9);
	QCOMPARE(index1[1], 18);
	QCOMPARE(index1[2], 27);
	
	// Test with different dimensions
	istd::CVarIndex index3(5);
	index3[0] = 10;
	index3[1] = 10;
	index3[2] = 10;
	index3[3] = 10;
	index3[4] = 10;
	
	istd::CVarIndex index4(2);
	index4[0] = 1;
	index4[1] = 1;
	
	index3 -= index4;
	
	// Only first 2 elements should be affected
	QCOMPARE(index3[0], 9);
	QCOMPARE(index3[1], 9);
	QCOMPARE(index3[2], 10);
	QCOMPARE(index3[3], 10);
	QCOMPARE(index3[4], 10);
}

// Hash function test

void CVarIndexTestRunner::testQHash()
{
	istd::CVarIndex index1(3);
	index1[0] = 1;
	index1[1] = 2;
	index1[2] = 3;
	
	istd::CVarIndex index2(3);
	index2[0] = 1;
	index2[1] = 2;
	index2[2] = 3;
	
	istd::CVarIndex index3(3);
	index3[0] = 1;
	index3[1] = 2;
	index3[2] = 4;
	
	// Equal indices should have same hash
	uint hash1 = qHash(index1);
	uint hash2 = qHash(index2);
	QCOMPARE(hash1, hash2);
	
	// Different indices will likely have different hashes (but not guaranteed)
	uint hash3 = qHash(index3);
	Q_UNUSED(hash3); // Just verify qHash doesn't crash
	
	// Test with seed - should return a value without crashing
	uint hashWithSeed = qHash(index1, 42);
	Q_UNUSED(hashWithSeed);
}


I_ADD_TEST(CVarIndexTestRunner);
