// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TArrayTest.h"


// protected slots

void TArrayTest::initTestCase()
{
}


void TArrayTest::DefaultConstructorTest()
{
	// Test 2D array
	istd::TArray<int, 2> array2d;
	QVERIFY(array2d.IsEmpty());
	QCOMPARE(array2d.GetDimensionsCount(), 2);
	QVERIFY(array2d.IsDimensionsCountFixed());
	
	// Test 3D array
	istd::TArray<double, 3> array3d;
	QVERIFY(array3d.IsEmpty());
	QCOMPARE(array3d.GetDimensionsCount(), 3);
}


void TArrayTest::SizeConstructorTest()
{
	// Test 2D array with sizes 3x4
	istd::TIndex<2> sizes2d;
	sizes2d[0] = 3;
	sizes2d[1] = 4;
	istd::TArray<int, 2> array2d(sizes2d, 42);
	
	QVERIFY(!array2d.IsEmpty());
	QCOMPARE(array2d.GetSize(0), 3);
	QCOMPARE(array2d.GetSize(1), 4);
	
	// Check that all elements are initialized to 42
	istd::TIndex<2> index2d;
	index2d[0] = 0;
	index2d[1] = 0;
	QCOMPARE(array2d.GetAt(index2d), 42);
	
	// Test 3D array with sizes 2x3x4
	istd::TIndex<3> sizes3d;
	sizes3d[0] = 2;
	sizes3d[1] = 3;
	sizes3d[2] = 4;
	istd::TArray<double, 3> array3d(sizes3d, 3.14);
	
	QVERIFY(!array3d.IsEmpty());
	QCOMPARE(array3d.GetSize(0), 2);
	QCOMPARE(array3d.GetSize(1), 3);
	QCOMPARE(array3d.GetSize(2), 4);
}


void TArrayTest::CopyConstructorTest()
{
	istd::TIndex<2> sizes;
	sizes[0] = 2;
	sizes[1] = 3;
	istd::TArray<int, 2> original(sizes, 10);
	
	// Set a specific value
	istd::TIndex<2> index;
	index[0] = 1;
	index[1] = 2;
	original.SetAt(index, 99);
	
	// Copy the array
	istd::TArray<int, 2> copy(original);
	
	QCOMPARE(copy.GetSize(0), 2);
	QCOMPARE(copy.GetSize(1), 3);
	QCOMPARE(copy.GetAt(index), 99);
}


void TArrayTest::IsEmptyTest()
{
	istd::TArray<int, 2> array;
	QVERIFY(array.IsEmpty());
	
	istd::TIndex<2> sizes;
	sizes[0] = 2;
	sizes[1] = 3;
	array.SetSizes(sizes);
	QVERIFY(!array.IsEmpty());
	
	array.Reset();
	QVERIFY(array.IsEmpty());
}


void TArrayTest::GetSetSizesTest()
{
	istd::TArray<int, 2> array;
	
	istd::TIndex<2> sizes;
	sizes[0] = 5;
	sizes[1] = 7;
	bool result = array.SetSizes(sizes);
	QVERIFY(result);
	
	const istd::TIndex<2>& retrievedSizes = array.GetSizes();
	QCOMPARE(retrievedSizes[0], 5);
	QCOMPARE(retrievedSizes[1], 7);
	
	// Test individual size setting
	array.SetSize(0, 3);
	QCOMPARE(array.GetSize(0), 3);
	QCOMPARE(array.GetSize(1), 7); // Should remain unchanged
}


void TArrayTest::GetSetElementTest()
{
	istd::TIndex<2> sizes;
	sizes[0] = 3;
	sizes[1] = 3;
	istd::TArray<int, 2> array(sizes);
	
	// Set elements at different positions
	istd::TIndex<2> index1;
	index1[0] = 0;
	index1[1] = 0;
	
	istd::TIndex<2> index2;
	index2[0] = 1;
	index2[1] = 2;
	
	istd::TIndex<2> index3;
	index3[0] = 2;
	index3[1] = 1;
	
	array.SetAt(index1, 11);
	array.SetAt(index2, 22);
	array.SetAt(index3, 33);
	
	// Get elements back
	QCOMPARE(array.GetAt(index1), 11);
	QCOMPARE(array.GetAt(index2), 22);
	QCOMPARE(array.GetAt(index3), 33);
	
	// Test GetAtRef
	array.GetAtRef(index1) = 99;
	QCOMPARE(array.GetAt(index1), 99);
}


void TArrayTest::SetAllElementsTest()
{
	istd::TIndex<2> sizes;
	sizes[0] = 3;
	sizes[1] = 4;
	istd::TArray<int, 2> array(sizes);
	
	// Set all elements to a specific value
	array.SetAllElements(7);
	
	// Verify all elements are set
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			istd::TIndex<2> index;
			index[0] = i;
			index[1] = j;
			QCOMPARE(array.GetAt(index), 7);
		}
	}
}


void TArrayTest::IteratorTest()
{
	istd::TIndex<2> sizes;
	sizes[0] = 2;
	sizes[1] = 3;
	istd::TArray<int, 2> array(sizes);
	
	// Set values using direct access
	int value = 1;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			istd::TIndex<2> index;
			index[0] = i;
			index[1] = j;
			array.SetAt(index, value++);
		}
	}
	
	// Verify using iterator
	int expectedValue = 1;
	auto it = array.Begin();
	auto end = array.End();
	
	while (it != end) {
		QCOMPARE(*it, expectedValue);
		expectedValue++;
		++it;
	}
	
	QCOMPARE(expectedValue, 7); // Should have iterated over 6 elements
}


void TArrayTest::cleanupTestCase()
{
}


I_ADD_TEST(TArrayTest);
