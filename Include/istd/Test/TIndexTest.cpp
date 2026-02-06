// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TIndexTest.h"


// protected slots

void TIndexTest::initTestCase()
{
}


void TIndexTest::DefaultConstructorTest()
{
	istd::TIndex<3> index;
	
	QCOMPARE(index.GetDimensionsCount(), 3);
	QVERIFY(index.IsDimensionsCountFixed());
	QVERIFY(index.IsZero());
	QVERIFY(index.IsValid());
	
	// All components should be 0
	QCOMPARE(index[0], 0);
	QCOMPARE(index[1], 0);
	QCOMPARE(index[2], 0);
}


void TIndexTest::ValueConstructorTest()
{
	istd::TIndex<4> index(5);
	
	QCOMPARE(index.GetDimensionsCount(), 4);
	QVERIFY(!index.IsZero());
	QVERIFY(index.IsValid());
	
	// All components should be 5
	QCOMPARE(index[0], 5);
	QCOMPARE(index[1], 5);
	QCOMPARE(index[2], 5);
	QCOMPARE(index[3], 5);
}


void TIndexTest::CopyConstructorTest()
{
	istd::TIndex<3> original;
	original[0] = 1;
	original[1] = 2;
	original[2] = 3;
	
	istd::TIndex<3> copy(original);
	
	QCOMPARE(copy[0], 1);
	QCOMPARE(copy[1], 2);
	QCOMPARE(copy[2], 3);
	QVERIFY(copy == original);
}


void TIndexTest::IsValidTest()
{
	istd::TIndex<2> validIndex;
	validIndex[0] = 5;
	validIndex[1] = 10;
	QVERIFY(validIndex.IsValid());
	
	istd::TIndex<2> invalidIndex;
	invalidIndex[0] = -1;
	invalidIndex[1] = 5;
	QVERIFY(!invalidIndex.IsValid());
}


void TIndexTest::IsZeroTest()
{
	istd::TIndex<3> zeroIndex;
	QVERIFY(zeroIndex.IsZero());
	
	istd::TIndex<3> nonZeroIndex;
	nonZeroIndex[0] = 0;
	nonZeroIndex[1] = 1;
	nonZeroIndex[2] = 0;
	QVERIFY(!nonZeroIndex.IsZero());
}


void TIndexTest::IsSizeEmptyTest()
{
	istd::TIndex<2> validSize;
	validSize[0] = 5;
	validSize[1] = 10;
	QVERIFY(!validSize.IsSizeEmpty());
	
	istd::TIndex<2> emptySize1;
	emptySize1[0] = 0;
	emptySize1[1] = 10;
	QVERIFY(emptySize1.IsSizeEmpty());
	
	istd::TIndex<2> emptySize2;
	emptySize2[0] = 5;
	emptySize2[1] = -1;
	QVERIFY(emptySize2.IsSizeEmpty());
}


void TIndexTest::GetSetAtTest()
{
	istd::TIndex<3> index;
	
	index.SetAt(0, 10);
	index.SetAt(1, 20);
	index.SetAt(2, 30);
	
	QCOMPARE(index.GetAt(0), 10);
	QCOMPARE(index.GetAt(1), 20);
	QCOMPARE(index.GetAt(2), 30);
}


void TIndexTest::ArrayAccessTest()
{
	istd::TIndex<3> index;
	
	// Test write access
	index[0] = 100;
	index[1] = 200;
	index[2] = 300;
	
	// Test read access
	QCOMPARE(index[0], 100);
	QCOMPARE(index[1], 200);
	QCOMPARE(index[2], 300);
}


void TIndexTest::ComparisonOperatorsTest()
{
	istd::TIndex<2> index1;
	index1[0] = 5;
	index1[1] = 10;
	
	istd::TIndex<2> index2;
	index2[0] = 5;
	index2[1] = 10;
	
	istd::TIndex<2> index3;
	index3[0] = 3;
	index3[1] = 7;
	
	// Test equality
	QVERIFY(index1 == index2);
	QVERIFY(!(index1 == index3));
	
	// Test inequality
	QVERIFY(index1 != index3);
	QVERIFY(!(index1 != index2));
	
	// Test less than (lexicographic comparison)
	QVERIFY(index3 < index1);
	QVERIFY(!(index1 < index2));
}


void TIndexTest::ArithmeticOperatorsTest()
{
	istd::TIndex<2> index1;
	index1[0] = 5;
	index1[1] = 10;
	
	istd::TIndex<2> index2;
	index2[0] = 3;
	index2[1] = 7;
	
	// Test addition
	istd::TIndex<2> sum = index1 + index2;
	QCOMPARE(sum[0], 8);
	QCOMPARE(sum[1], 17);
	
	// Test subtraction
	istd::TIndex<2> diff = index1 - index2;
	QCOMPARE(diff[0], 2);
	QCOMPARE(diff[1], 3);
	
	// Test +=
	istd::TIndex<2> index3 = index1;
	index3 += index2;
	QCOMPARE(index3[0], 8);
	QCOMPARE(index3[1], 17);
	
	// Test -=
	istd::TIndex<2> index4 = index1;
	index4 -= index2;
	QCOMPARE(index4[0], 2);
	QCOMPARE(index4[1], 3);
}


void TIndexTest::cleanupTestCase()
{
}


I_ADD_TEST(TIndexTest);
