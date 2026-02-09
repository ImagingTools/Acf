// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TMatrixTest.h"


// protected slots

void TMatrixTest::initTestCase()
{
}


void TMatrixTest::DefaultConstructorTest()
{
	// Test 2x2 matrix
	imath::TMatrix<2, 2, double> mat2x2;
	mat2x2.Clear();
	QVERIFY(mat2x2.GetAt(0, 0) == 0.0);
	QVERIFY(mat2x2.GetAt(1, 1) == 0.0);
	
	// Test 3x3 matrix
	imath::TMatrix<3, 3, double> mat3x3;
	mat3x3.Clear();
	QVERIFY(mat3x3.GetAt(0, 0) == 0.0);
	QVERIFY(mat3x3.GetAt(2, 2) == 0.0);
}


void TMatrixTest::InitModeConstructorTest()
{
	// Test zero initialization
	imath::TMatrix<3, 3, double> matZero(imath::TMatrix<3, 3, double>::MIM_ZERO);
	QVERIFY(matZero.GetAt(0, 0) == 0.0);
	QVERIFY(matZero.GetAt(1, 1) == 0.0);
	QVERIFY(matZero.GetAt(2, 2) == 0.0);
	
	// Test ones initialization
	imath::TMatrix<3, 3, double> matOnes(imath::TMatrix<3, 3, double>::MIM_ONES);
	QVERIFY(matOnes.GetAt(0, 0) == 1.0);
	QVERIFY(matOnes.GetAt(1, 1) == 1.0);
	QVERIFY(matOnes.GetAt(0, 1) == 1.0);
	
	// Test identity initialization
	imath::TMatrix<3, 3, double> matIdentity(imath::TMatrix<3, 3, double>::MIM_IDENTITY);
	QVERIFY(matIdentity.GetAt(0, 0) == 1.0);
	QVERIFY(matIdentity.GetAt(1, 1) == 1.0);
	QVERIFY(matIdentity.GetAt(2, 2) == 1.0);
	QVERIFY(matIdentity.GetAt(0, 1) == 0.0);
	QVERIFY(matIdentity.GetAt(1, 0) == 0.0);
}


void TMatrixTest::CopyConstructorTest()
{
	// Create and initialize matrix
	imath::TMatrix<2, 2, double> mat1;
	mat1.SetAt(0, 0, 1.0);
	mat1.SetAt(0, 1, 2.0);
	mat1.SetAt(1, 0, 3.0);
	mat1.SetAt(1, 1, 4.0);
	
	// Test copy constructor
	imath::TMatrix<2, 2, double> mat2(mat1);
	QVERIFY(mat2.GetAt(0, 0) == 1.0);
	QVERIFY(mat2.GetAt(0, 1) == 2.0);
	QVERIFY(mat2.GetAt(1, 0) == 3.0);
	QVERIFY(mat2.GetAt(1, 1) == 4.0);
}


void TMatrixTest::ClearResetTest()
{
	imath::TMatrix<2, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(1, 1, 2.0);
	
	// Test Clear
	mat.Clear();
	QVERIFY(mat.GetAt(0, 0) == 0.0);
	QVERIFY(mat.GetAt(1, 1) == 0.0);
	
	// Set values again and test Reset
	mat.SetAt(0, 0, 3.0);
	mat.SetAt(1, 1, 4.0);
	mat.Reset();
	QVERIFY(mat.GetAt(0, 0) == 0.0);
	QVERIFY(mat.GetAt(1, 1) == 0.0);
}


void TMatrixTest::GetSetElementTest()
{
	imath::TMatrix<3, 3, double> mat;
	
	// Test SetAt with x, y
	mat.SetAt(0, 0, 1.5);
	mat.SetAt(1, 2, 2.5);
	mat.SetAt(2, 1, 3.5);
	
	// Test GetAt
	QVERIFY(mat.GetAt(0, 0) == 1.5);
	QVERIFY(mat.GetAt(1, 2) == 2.5);
	QVERIFY(mat.GetAt(2, 1) == 3.5);
	
	// Test GetAtRef
	mat.GetAtRef(1, 1) = 7.5;
	QVERIFY(mat.GetAt(1, 1) == 7.5);
	
	// Test with Index2d
	istd::CIndex2d index(0, 1);
	mat.SetAt(index, 9.5);
	QVERIFY(mat.GetAt(index) == 9.5);
}


void TMatrixTest::InitToIdentityTest()
{
	imath::TMatrix<3, 3, double> mat;
	mat.InitToIdentity();
	
	// Check diagonal elements
	QVERIFY(mat.GetAt(0, 0) == 1.0);
	QVERIFY(mat.GetAt(1, 1) == 1.0);
	QVERIFY(mat.GetAt(2, 2) == 1.0);
	
	// Check off-diagonal elements
	QVERIFY(mat.GetAt(0, 1) == 0.0);
	QVERIFY(mat.GetAt(0, 2) == 0.0);
	QVERIFY(mat.GetAt(1, 0) == 0.0);
	QVERIFY(mat.GetAt(1, 2) == 0.0);
	QVERIFY(mat.GetAt(2, 0) == 0.0);
	QVERIFY(mat.GetAt(2, 1) == 0.0);
}


void TMatrixTest::GetMinMaxElementTest()
{
	imath::TMatrix<2, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 5.0);
	mat.SetAt(1, 0, -2.0);
	mat.SetAt(1, 1, 3.0);
	
	QVERIFY(mat.GetMinElement() == -2.0);
	QVERIFY(mat.GetMaxElement() == 5.0);
}


void TMatrixTest::NegationTest()
{
	imath::TMatrix<2, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, -2.0);
	mat.SetAt(1, 0, 3.0);
	mat.SetAt(1, 1, -4.0);
	
	imath::TMatrix<2, 2, double> result;
	mat.GetNegated(result);
	
	QVERIFY(result.GetAt(0, 0) == -1.0);
	QVERIFY(result.GetAt(0, 1) == 2.0);
	QVERIFY(result.GetAt(1, 0) == -3.0);
	QVERIFY(result.GetAt(1, 1) == 4.0);
}


void TMatrixTest::AdditionTest()
{
	imath::TMatrix<2, 2, double> mat1;
	mat1.SetAt(0, 0, 1.0);
	mat1.SetAt(0, 1, 2.0);
	mat1.SetAt(1, 0, 3.0);
	mat1.SetAt(1, 1, 4.0);
	
	imath::TMatrix<2, 2, double> mat2;
	mat2.SetAt(0, 0, 5.0);
	mat2.SetAt(0, 1, 6.0);
	mat2.SetAt(1, 0, 7.0);
	mat2.SetAt(1, 1, 8.0);
	
	imath::TMatrix<2, 2, double> result;
	mat1.GetAdded(mat2, result);
	
	QVERIFY(result.GetAt(0, 0) == 6.0);
	QVERIFY(result.GetAt(0, 1) == 8.0);
	QVERIFY(result.GetAt(1, 0) == 10.0);
	QVERIFY(result.GetAt(1, 1) == 12.0);
}


void TMatrixTest::SubtractionTest()
{
	imath::TMatrix<2, 2, double> mat1;
	mat1.SetAt(0, 0, 10.0);
	mat1.SetAt(0, 1, 8.0);
	mat1.SetAt(1, 0, 6.0);
	mat1.SetAt(1, 1, 4.0);
	
	imath::TMatrix<2, 2, double> mat2;
	mat2.SetAt(0, 0, 1.0);
	mat2.SetAt(0, 1, 2.0);
	mat2.SetAt(1, 0, 3.0);
	mat2.SetAt(1, 1, 4.0);
	
	imath::TMatrix<2, 2, double> result;
	mat1.GetSubstracted(mat2, result);
	
	QVERIFY(result.GetAt(0, 0) == 9.0);
	QVERIFY(result.GetAt(0, 1) == 6.0);
	QVERIFY(result.GetAt(1, 0) == 3.0);
	QVERIFY(result.GetAt(1, 1) == 0.0);
}


void TMatrixTest::MatrixMultiplicationTest()
{
	// Test 2x2 * 2x2 = 2x2
	imath::TMatrix<2, 2, double> mat1;
	mat1.SetAt(0, 0, 1.0);
	mat1.SetAt(0, 1, 2.0);
	mat1.SetAt(1, 0, 3.0);
	mat1.SetAt(1, 1, 4.0);
	
	imath::TMatrix<2, 2, double> mat2;
	mat2.SetAt(0, 0, 5.0);
	mat2.SetAt(0, 1, 6.0);
	mat2.SetAt(1, 0, 7.0);
	mat2.SetAt(1, 1, 8.0);
	
	imath::TMatrix<2, 2, double> result;
	mat1.GetMultiplied(mat2, result);
	
	// [1 3] * [5 7] = [1*5+3*7  1*6+3*8] = [26 30]
	// [2 4]   [6 8]   [2*5+4*7  2*6+4*8]   [38 44]
	QVERIFY(qAbs(result.GetAt(0, 0) - 26.0) < 1e-10);
	QVERIFY(qAbs(result.GetAt(0, 1) - 38.0) < 1e-10);
	QVERIFY(qAbs(result.GetAt(1, 0) - 30.0) < 1e-10);
	QVERIFY(qAbs(result.GetAt(1, 1) - 44.0) < 1e-10);
	
	// Test identity multiplication
	imath::TMatrix<2, 2, double> identity(imath::TMatrix<2, 2, double>::MIM_IDENTITY);
	imath::TMatrix<2, 2, double> result2;
	mat1.GetMultiplied(identity, result2);
	
	QVERIFY(result2.GetAt(0, 0) == mat1.GetAt(0, 0));
	QVERIFY(result2.GetAt(0, 1) == mat1.GetAt(0, 1));
	QVERIFY(result2.GetAt(1, 0) == mat1.GetAt(1, 0));
	QVERIFY(result2.GetAt(1, 1) == mat1.GetAt(1, 1));
}


void TMatrixTest::VectorMultiplicationTest()
{
	// Test matrix * vector
	imath::TMatrix<2, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 2.0);
	mat.SetAt(1, 0, 3.0);
	mat.SetAt(1, 1, 4.0);
	
	imath::TVector<2, double> vec;
	vec.SetElement(0, 5.0);
	vec.SetElement(1, 6.0);
	
	imath::TVector<2, double> result;
	mat.GetMultiplied(vec, result);
	
	// [1 3] * [5] = [1*5+3*6] = [23]
	// [2 4]   [6]   [2*5+4*6]   [34]
	QVERIFY(qAbs(result.GetElement(0) - 23.0) < 1e-10);
	QVERIFY(qAbs(result.GetElement(1) - 34.0) < 1e-10);
	
	// Test with return value
	imath::TVector<2, double> result2 = mat.GetMultiplied(vec);
	QVERIFY(qAbs(result2.GetElement(0) - 23.0) < 1e-10);
	QVERIFY(qAbs(result2.GetElement(1) - 34.0) < 1e-10);
}


void TMatrixTest::ScalarMultiplicationTest()
{
	imath::TMatrix<2, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 2.0);
	mat.SetAt(1, 0, 3.0);
	mat.SetAt(1, 1, 4.0);
	
	imath::TMatrix<2, 2, double> result;
	mat.GetScaled(2.0, result);
	
	QVERIFY(result.GetAt(0, 0) == 2.0);
	QVERIFY(result.GetAt(0, 1) == 4.0);
	QVERIFY(result.GetAt(1, 0) == 6.0);
	QVERIFY(result.GetAt(1, 1) == 8.0);
}


void TMatrixTest::TransposeTest()
{
	// Test with 2x3 matrix
	imath::TMatrix<2, 3, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 2.0);
	mat.SetAt(0, 2, 3.0);
	mat.SetAt(1, 0, 4.0);
	mat.SetAt(1, 1, 5.0);
	mat.SetAt(1, 2, 6.0);
	
	// Get transposed (3x2)
	imath::TMatrix<3, 2, double> transposed;
	mat.GetTransposed(transposed);
	
	QVERIFY(transposed.GetAt(0, 0) == 1.0);
	QVERIFY(transposed.GetAt(0, 1) == 4.0);
	QVERIFY(transposed.GetAt(1, 0) == 2.0);
	QVERIFY(transposed.GetAt(1, 1) == 5.0);
	QVERIFY(transposed.GetAt(2, 0) == 3.0);
	QVERIFY(transposed.GetAt(2, 1) == 6.0);
	
	// Test with return value
	imath::TMatrix<3, 2, double> transposed2 = mat.GetTransposed();
	QVERIFY(transposed2.GetAt(0, 0) == 1.0);
	QVERIFY(transposed2.GetAt(2, 1) == 6.0);
	
	// Test in-place transpose (square matrix only)
	imath::TMatrix<2, 2, double> squareMat;
	squareMat.SetAt(0, 0, 1.0);
	squareMat.SetAt(0, 1, 2.0);
	squareMat.SetAt(1, 0, 3.0);
	squareMat.SetAt(1, 1, 4.0);
	squareMat.Transpose();
	QVERIFY(squareMat.GetAt(0, 0) == 1.0);
	QVERIFY(squareMat.GetAt(0, 1) == 3.0);
	QVERIFY(squareMat.GetAt(1, 0) == 2.0);
	QVERIFY(squareMat.GetAt(1, 1) == 4.0);
}


void TMatrixTest::GetTraceTest()
{
	// Test with 3x3 matrix
	imath::TMatrix<3, 3, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 2.0);
	mat.SetAt(0, 2, 3.0);
	mat.SetAt(1, 0, 4.0);
	mat.SetAt(1, 1, 5.0);
	mat.SetAt(1, 2, 6.0);
	mat.SetAt(2, 0, 7.0);
	mat.SetAt(2, 1, 8.0);
	mat.SetAt(2, 2, 9.0);
	
	// Trace = sum of diagonal elements = 1 + 5 + 9 = 15
	double trace = mat.GetTrace();
	QVERIFY(qAbs(trace - 15.0) < 1e-10);
	
	// Test identity matrix
	imath::TMatrix<3, 3, double> identity(imath::TMatrix<3, 3, double>::MIM_IDENTITY);
	QVERIFY(qAbs(identity.GetTrace() - 3.0) < 1e-10);
}


void TMatrixTest::FrobeniusNormTest()
{
	// Test with 2x2 matrix
	imath::TMatrix<2, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 2.0);
	mat.SetAt(1, 0, 3.0);
	mat.SetAt(1, 1, 4.0);
	
	// Frobenius norm^2 = 1^2 + 2^2 + 3^2 + 4^2 = 30
	double norm2 = mat.GetFrobeniusNorm2();
	QVERIFY(qAbs(norm2 - 30.0) < 1e-10);
	
	// Frobenius norm = sqrt(30)
	double norm = mat.GetFrobeniusNorm();
	QVERIFY(qAbs(norm - qSqrt(30.0)) < 1e-10);
}


void TMatrixTest::GetColumnRowVectorTest()
{
	imath::TMatrix<3, 2, double> mat;
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(0, 1, 2.0);
	mat.SetAt(1, 0, 3.0);
	mat.SetAt(1, 1, 4.0);
	mat.SetAt(2, 0, 5.0);
	mat.SetAt(2, 1, 6.0);
	
	// Get column vector (height = 2)
	imath::TVector<2, double> col0;
	mat.GetColumnVector(0, col0);
	QVERIFY(col0.GetElement(0) == 1.0);
	QVERIFY(col0.GetElement(1) == 2.0);
	
	imath::TVector<2, double> col2;
	mat.GetColumnVector(2, col2);
	QVERIFY(col2.GetElement(0) == 5.0);
	QVERIFY(col2.GetElement(1) == 6.0);
	
	// Get row vector (width = 3)
	imath::TVector<3, double> row0;
	mat.GetRowVector(0, row0);
	QVERIFY(row0.GetElement(0) == 1.0);
	QVERIFY(row0.GetElement(1) == 3.0);
	QVERIFY(row0.GetElement(2) == 5.0);
	
	imath::TVector<3, double> row1;
	mat.GetRowVector(1, row1);
	QVERIFY(row1.GetElement(0) == 2.0);
	QVERIFY(row1.GetElement(1) == 4.0);
	QVERIFY(row1.GetElement(2) == 6.0);
}


void TMatrixTest::SetColumnRowVectorTest()
{
	imath::TMatrix<3, 2, double> mat;
	mat.Clear();
	
	// Set column vector
	imath::TVector<2, double> col;
	col.SetElement(0, 10.0);
	col.SetElement(1, 20.0);
	mat.SetColumnVector(1, col);
	
	QVERIFY(mat.GetAt(1, 0) == 10.0);
	QVERIFY(mat.GetAt(1, 1) == 20.0);
	
	// Verify the column was set correctly by reading it back
	imath::TVector<2, double> colVerify;
	mat.GetColumnVector(1, colVerify);
	QVERIFY(colVerify.GetElement(0) == 10.0);
	QVERIFY(colVerify.GetElement(1) == 20.0);
	
	// Note: TMatrix does not have SetRowVector method
	// Rows can only be set by setting individual elements or columns
	// Set elements to form a row pattern for verification
	mat.SetAt(0, 0, 1.0);
	mat.SetAt(1, 0, 2.0);
	mat.SetAt(2, 0, 3.0);
	
	// Verify row was set correctly
	QVERIFY(mat.GetAt(0, 0) == 1.0);
	QVERIFY(mat.GetAt(1, 0) == 2.0);
	QVERIFY(mat.GetAt(2, 0) == 3.0);
}


void TMatrixTest::cleanupTestCase()
{
}


I_ADD_TEST(TMatrixTest);
