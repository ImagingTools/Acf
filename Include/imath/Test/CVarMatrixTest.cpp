// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CVarMatrixTest.h"

// ACF includes
#include <istd/CIndex2d.h>
#include <imath/CVarVector.h>


// protected slots

void CVarMatrixTest::initTestCase()
{
}


void CVarMatrixTest::DefaultConstructorTest()
{
	imath::CVarMatrix matrix;
	
	QVERIFY(matrix.IsEmpty());
	QCOMPARE(matrix.GetDimensionsCount(), 2);
}


void CVarMatrixTest::SizeConstructorTest()
{
	// Test creating a 3x4 matrix
	istd::CIndex2d size(3, 4);
	imath::CVarMatrix matrix(size);
	
	QVERIFY(!matrix.IsEmpty());
	QCOMPARE(matrix.GetSize(0), 3);
	QCOMPARE(matrix.GetSize(1), 4);
}


void CVarMatrixTest::CopyConstructorTest()
{
	// Create original matrix
	istd::CIndex2d size(2, 3);
	imath::CVarMatrix original(size);
	
	// Set some values
	original.GetElementRef(0, 0) = 1.0;
	original.GetElementRef(1, 2) = 5.5;
	
	// Copy the matrix
	imath::CVarMatrix copy(original);
	
	QCOMPARE(copy.GetSize(0), 2);
	QCOMPARE(copy.GetSize(1), 3);
	QCOMPARE(copy.GetElementAt(0, 0), 1.0);
	QCOMPARE(copy.GetElementAt(1, 2), 5.5);
}


void CVarMatrixTest::ClearTest()
{
	istd::CIndex2d size(2, 2);
	imath::CVarMatrix matrix(size);
	
	// Set non-zero values
	matrix.GetElementRef(0, 0) = 5.0;
	matrix.GetElementRef(1, 1) = 10.0;
	
	// Clear the matrix
	matrix.Clear();
	
	// All elements should be zero
	QCOMPARE(matrix.GetElementAt(0, 0), 0.0);
	QCOMPARE(matrix.GetElementAt(0, 1), 0.0);
	QCOMPARE(matrix.GetElementAt(1, 0), 0.0);
	QCOMPARE(matrix.GetElementAt(1, 1), 0.0);
}


void CVarMatrixTest::InitToIdentityTest()
{
	imath::CVarMatrix matrix;
	matrix.InitToIdentity(3);
	
	// Check size
	QCOMPARE(matrix.GetSize(0), 3);
	QCOMPARE(matrix.GetSize(1), 3);
	
	// Check identity matrix: diagonal = 1, off-diagonal = 0
	QCOMPARE(matrix.GetElementAt(0, 0), 1.0);
	QCOMPARE(matrix.GetElementAt(1, 1), 1.0);
	QCOMPARE(matrix.GetElementAt(2, 2), 1.0);
	
	QCOMPARE(matrix.GetElementAt(0, 1), 0.0);
	QCOMPARE(matrix.GetElementAt(0, 2), 0.0);
	QCOMPARE(matrix.GetElementAt(1, 0), 0.0);
	QCOMPARE(matrix.GetElementAt(1, 2), 0.0);
	QCOMPARE(matrix.GetElementAt(2, 0), 0.0);
	QCOMPARE(matrix.GetElementAt(2, 1), 0.0);
}


void CVarMatrixTest::GetSetElementTest()
{
	istd::CIndex2d size(3, 3);
	imath::CVarMatrix matrix(size);
	
	// Test GetElementRef and setting
	matrix.GetElementRef(0, 0) = 1.5;
	matrix.GetElementRef(1, 2) = 3.7;
	matrix.GetElementRef(2, 1) = -2.3;
	
	// Test GetElementAt
	QCOMPARE(matrix.GetElementAt(0, 0), 1.5);
	QCOMPARE(matrix.GetElementAt(1, 2), 3.7);
	QCOMPARE(matrix.GetElementAt(2, 1), -2.3);
}


void CVarMatrixTest::GetMinMaxElementTest()
{
	istd::CIndex2d size(2, 3);
	imath::CVarMatrix matrix(size);
	
	matrix.GetElementRef(0, 0) = 5.0;
	matrix.GetElementRef(0, 1) = -3.0;
	matrix.GetElementRef(0, 2) = 7.5;
	matrix.GetElementRef(1, 0) = 2.0;
	matrix.GetElementRef(1, 1) = 10.0;
	matrix.GetElementRef(1, 2) = -1.0;
	
	QCOMPARE(matrix.GetMaxElement(), 10.0);
	QCOMPARE(matrix.GetMinElement(), -3.0);
}


void CVarMatrixTest::AdditionTest()
{
	istd::CIndex2d size(2, 2);
	imath::CVarMatrix m1(size);
	imath::CVarMatrix m2(size);
	
	// Set values for m1
	m1.GetElementRef(0, 0) = 1.0;
	m1.GetElementRef(0, 1) = 2.0;
	m1.GetElementRef(1, 0) = 3.0;
	m1.GetElementRef(1, 1) = 4.0;
	
	// Set values for m2
	m2.GetElementRef(0, 0) = 5.0;
	m2.GetElementRef(0, 1) = 6.0;
	m2.GetElementRef(1, 0) = 7.0;
	m2.GetElementRef(1, 1) = 8.0;
	
	// Test operator+
	imath::CVarMatrix result = m1 + m2;
	
	QCOMPARE(result.GetElementAt(0, 0), 6.0);
	QCOMPARE(result.GetElementAt(0, 1), 8.0);
	QCOMPARE(result.GetElementAt(1, 0), 10.0);
	QCOMPARE(result.GetElementAt(1, 1), 12.0);
}


void CVarMatrixTest::SubtractionTest()
{
	istd::CIndex2d size(2, 2);
	imath::CVarMatrix m1(size);
	imath::CVarMatrix m2(size);
	
	// Set values
	m1.GetElementRef(0, 0) = 10.0;
	m1.GetElementRef(0, 1) = 8.0;
	m1.GetElementRef(1, 0) = 6.0;
	m1.GetElementRef(1, 1) = 4.0;
	
	m2.GetElementRef(0, 0) = 1.0;
	m2.GetElementRef(0, 1) = 2.0;
	m2.GetElementRef(1, 0) = 3.0;
	m2.GetElementRef(1, 1) = 4.0;
	
	// Test operator-
	imath::CVarMatrix result = m1 - m2;
	
	QCOMPARE(result.GetElementAt(0, 0), 9.0);
	QCOMPARE(result.GetElementAt(0, 1), 6.0);
	QCOMPARE(result.GetElementAt(1, 0), 3.0);
	QCOMPARE(result.GetElementAt(1, 1), 0.0);
}


void CVarMatrixTest::NegationTest()
{
	istd::CIndex2d size(2, 2);
	imath::CVarMatrix matrix(size);
	
	matrix.GetElementRef(0, 0) = 1.0;
	matrix.GetElementRef(0, 1) = -2.0;
	matrix.GetElementRef(1, 0) = 3.0;
	matrix.GetElementRef(1, 1) = -4.0;
	
	// Test unary operator-
	imath::CVarMatrix result = -matrix;
	
	QCOMPARE(result.GetElementAt(0, 0), -1.0);
	QCOMPARE(result.GetElementAt(0, 1), 2.0);
	QCOMPARE(result.GetElementAt(1, 0), -3.0);
	QCOMPARE(result.GetElementAt(1, 1), 4.0);
}


void CVarMatrixTest::MultiplicationTest()
{
	// Test matrix multiplication: [2x3] * [3x2] = [2x2]
	istd::CIndex2d size1(2, 3);
	istd::CIndex2d size2(3, 2);
	
	imath::CVarMatrix m1(size1);
	imath::CVarMatrix m2(size2);
	
	// m1 = [[1, 2, 3],
	//       [4, 5, 6]]
	m1.GetElementRef(0, 0) = 1.0;
	m1.GetElementRef(0, 1) = 2.0;
	m1.GetElementRef(0, 2) = 3.0;
	m1.GetElementRef(1, 0) = 4.0;
	m1.GetElementRef(1, 1) = 5.0;
	m1.GetElementRef(1, 2) = 6.0;
	
	// m2 = [[7, 8],
	//       [9, 10],
	//       [11, 12]]
	m2.GetElementRef(0, 0) = 7.0;
	m2.GetElementRef(0, 1) = 8.0;
	m2.GetElementRef(1, 0) = 9.0;
	m2.GetElementRef(1, 1) = 10.0;
	m2.GetElementRef(2, 0) = 11.0;
	m2.GetElementRef(2, 1) = 12.0;
	
	// result = m1 * m2
	// result = [[1*7+2*9+3*11, 1*8+2*10+3*12],
	//           [4*7+5*9+6*11, 4*8+5*10+6*12]]
	// result = [[58, 64],
	//           [139, 154]]
	imath::CVarMatrix result = m1 * m2;
	
	QCOMPARE(result.GetSize(0), 2);
	QCOMPARE(result.GetSize(1), 2);
	QCOMPARE(result.GetElementAt(0, 0), 58.0);
	QCOMPARE(result.GetElementAt(0, 1), 64.0);
	QCOMPARE(result.GetElementAt(1, 0), 139.0);
	QCOMPARE(result.GetElementAt(1, 1), 154.0);
}


void CVarMatrixTest::ScalarMultiplicationTest()
{
	istd::CIndex2d size(2, 2);
	imath::CVarMatrix matrix(size);
	
	matrix.GetElementRef(0, 0) = 2.0;
	matrix.GetElementRef(0, 1) = 4.0;
	matrix.GetElementRef(1, 0) = 6.0;
	matrix.GetElementRef(1, 1) = 8.0;
	
	// Test scalar multiplication from right
	imath::CVarMatrix result1 = matrix * 0.5;
	
	QCOMPARE(result1.GetElementAt(0, 0), 1.0);
	QCOMPARE(result1.GetElementAt(0, 1), 2.0);
	QCOMPARE(result1.GetElementAt(1, 0), 3.0);
	QCOMPARE(result1.GetElementAt(1, 1), 4.0);
	
	// Test scalar multiplication from left
	imath::CVarMatrix result2 = 2.0 * matrix;
	
	QCOMPARE(result2.GetElementAt(0, 0), 4.0);
	QCOMPARE(result2.GetElementAt(0, 1), 8.0);
	QCOMPARE(result2.GetElementAt(1, 0), 12.0);
	QCOMPARE(result2.GetElementAt(1, 1), 16.0);
}


void CVarMatrixTest::TransposeTest()
{
	istd::CIndex2d size(2, 3);
	imath::CVarMatrix matrix(size);
	
	// Original: [[1, 2, 3],
	//            [4, 5, 6]]
	matrix.GetElementRef(0, 0) = 1.0;
	matrix.GetElementRef(0, 1) = 2.0;
	matrix.GetElementRef(0, 2) = 3.0;
	matrix.GetElementRef(1, 0) = 4.0;
	matrix.GetElementRef(1, 1) = 5.0;
	matrix.GetElementRef(1, 2) = 6.0;
	
	// Get transposed matrix using GetTransposed(result) method
	imath::CVarMatrix transposed;
	matrix.GetTransposed(transposed);
	
	// Transposed should be 3x2: [[1, 4],
	//                             [2, 5],
	//                             [3, 6]]
	QCOMPARE(transposed.GetSize(0), 3);
	QCOMPARE(transposed.GetSize(1), 2);
	QCOMPARE(transposed.GetElementAt(0, 0), 1.0);
	QCOMPARE(transposed.GetElementAt(0, 1), 4.0);
	QCOMPARE(transposed.GetElementAt(1, 0), 2.0);
	QCOMPARE(transposed.GetElementAt(1, 1), 5.0);
	QCOMPARE(transposed.GetElementAt(2, 0), 3.0);
	QCOMPARE(transposed.GetElementAt(2, 1), 6.0);
	
	// Test in-place transpose
	imath::CVarMatrix matrix2(size);
	matrix2.GetElementRef(0, 0) = 1.0;
	matrix2.GetElementRef(1, 1) = 5.0;
	
	matrix2.Transpose();
	QCOMPARE(matrix2.GetSize(0), 3);
	QCOMPARE(matrix2.GetSize(1), 2);
}


void CVarMatrixTest::GetTraceTest()
{
	// Trace is only meaningful for square matrices
	istd::CIndex2d size(3, 3);
	imath::CVarMatrix matrix(size);
	
	matrix.InitToIdentity(3);
	matrix.GetElementRef(0, 0) = 2.0;
	matrix.GetElementRef(1, 1) = 3.0;
	matrix.GetElementRef(2, 2) = 4.0;
	
	// Trace = sum of diagonal elements = 2 + 3 + 4 = 9
	QCOMPARE(matrix.GetTrace(), 9.0);
}


void CVarMatrixTest::FrobeniusNormTest()
{
	istd::CIndex2d size(2, 2);
	imath::CVarMatrix matrix(size);
	
	// Matrix = [[3, 0],
	//           [0, 4]]
	matrix.GetElementRef(0, 0) = 3.0;
	matrix.GetElementRef(0, 1) = 0.0;
	matrix.GetElementRef(1, 0) = 0.0;
	matrix.GetElementRef(1, 1) = 4.0;
	
	// Frobenius norm squared = 3^2 + 0^2 + 0^2 + 4^2 = 9 + 16 = 25
	QCOMPARE(matrix.GetFrobeniusNorm2(), 25.0);
	
	// Frobenius norm = sqrt(25) = 5.0
	QCOMPARE(matrix.GetFrobeniusNorm(), 5.0);
}


void CVarMatrixTest::ComparisonOperatorsTest()
{
	// Note: CVarMatrix doesn't have operator== and operator!= implemented.
	// We test equality by comparing individual elements instead.
	
	istd::CIndex2d size(2, 2);
	
	imath::CVarMatrix m1(size);
	m1.GetElementRef(0, 0) = 1.0;
	m1.GetElementRef(0, 1) = 2.0;
	m1.GetElementRef(1, 0) = 3.0;
	m1.GetElementRef(1, 1) = 4.0;
	
	imath::CVarMatrix m2(size);
	m2.GetElementRef(0, 0) = 1.0;
	m2.GetElementRef(0, 1) = 2.0;
	m2.GetElementRef(1, 0) = 3.0;
	m2.GetElementRef(1, 1) = 4.0;
	
	imath::CVarMatrix m3(size);
	m3.GetElementRef(0, 0) = 1.0;
	m3.GetElementRef(0, 1) = 2.0;
	m3.GetElementRef(1, 0) = 3.0;
	m3.GetElementRef(1, 1) = 5.0; // Different
	
	// Test equality by comparing elements
	QVERIFY(m1.GetElementAt(0, 0) == m2.GetElementAt(0, 0));
	QVERIFY(m1.GetElementAt(0, 1) == m2.GetElementAt(0, 1));
	QVERIFY(m1.GetElementAt(1, 0) == m2.GetElementAt(1, 0));
	QVERIFY(m1.GetElementAt(1, 1) == m2.GetElementAt(1, 1));
	
	// Test inequality
	QVERIFY(m1.GetElementAt(1, 1) != m3.GetElementAt(1, 1));
}


void CVarMatrixTest::cleanupTestCase()
{
}


I_ADD_TEST(CVarMatrixTest);
