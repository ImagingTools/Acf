// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TVectorTest.h"


// protected slots

void TVectorTest::initTestCase()
{
}


void TVectorTest::DefaultConstructorTest()
{
	// Test 2D vector
	imath::TVector<2, double> vec2d;
	vec2d.Clear(); // Initialize for testing
	QVERIFY(vec2d.GetElement(0) == 0.0);
	QVERIFY(vec2d.GetElement(1) == 0.0);
	
	// Test 3D vector
	imath::TVector<3, double> vec3d;
	vec3d.Clear();
	QVERIFY(vec3d.GetElement(0) == 0.0);
	QVERIFY(vec3d.GetElement(1) == 0.0);
	QVERIFY(vec3d.GetElement(2) == 0.0);
}


void TVectorTest::CopyConstructorTest()
{
	// Create and initialize vector
	imath::TVector<3, double> vec1;
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 2.0);
	vec1.SetElement(2, 3.0);
	
	// Test copy constructor
	imath::TVector<3, double> vec2(vec1);
	QVERIFY(vec2.GetElement(0) == 1.0);
	QVERIFY(vec2.GetElement(1) == 2.0);
	QVERIFY(vec2.GetElement(2) == 3.0);
}


void TVectorTest::InitializerListConstructorTest()
{
	// Test with full initialization
	imath::TVector<3, double> vec1 = {1.0, 2.0, 3.0};
	QVERIFY(vec1.GetElement(0) == 1.0);
	QVERIFY(vec1.GetElement(1) == 2.0);
	QVERIFY(vec1.GetElement(2) == 3.0);
	
	// Test with partial initialization
	imath::TVector<3, double> vec2 = {4.0, 5.0};
	QVERIFY(vec2.GetElement(0) == 4.0);
	QVERIFY(vec2.GetElement(1) == 5.0);
	QVERIFY(vec2.GetElement(2) == 0.0); // Should be default initialized
	
	// Test integer vector
	imath::TVector<2, int> vec3 = {10, 20};
	QVERIFY(vec3.GetElement(0) == 10);
	QVERIFY(vec3.GetElement(1) == 20);
}


void TVectorTest::GetSetElementTest()
{
	imath::TVector<3, double> vec;
	
	// Test SetElement
	vec.SetElement(0, 1.5);
	vec.SetElement(1, 2.5);
	vec.SetElement(2, 3.5);
	
	// Test GetElement
	QVERIFY(vec.GetElement(0) == 1.5);
	QVERIFY(vec.GetElement(1) == 2.5);
	QVERIFY(vec.GetElement(2) == 3.5);
	
	// Test GetElementRef
	vec.GetElementRef(1) = 7.5;
	QVERIFY(vec.GetElement(1) == 7.5);
}


void TVectorTest::SetAllElementsTest()
{
	imath::TVector<4, double> vec;
	vec.SetAllElements(5.0);
	
	QVERIFY(vec.GetElement(0) == 5.0);
	QVERIFY(vec.GetElement(1) == 5.0);
	QVERIFY(vec.GetElement(2) == 5.0);
	QVERIFY(vec.GetElement(3) == 5.0);
}


void TVectorTest::ClearResetTest()
{
	imath::TVector<3, double> vec = {1.0, 2.0, 3.0};
	
	// Test Clear
	vec.Clear();
	QVERIFY(vec.GetElement(0) == 0.0);
	QVERIFY(vec.GetElement(1) == 0.0);
	QVERIFY(vec.GetElement(2) == 0.0);
	
	// Reset and test Reset
	vec.SetElement(0, 1.0);
	vec.SetElement(1, 2.0);
	vec.SetElement(2, 3.0);
	vec.Reset();
	QVERIFY(vec.GetElement(0) == 0.0);
	QVERIFY(vec.GetElement(1) == 0.0);
	QVERIFY(vec.GetElement(2) == 0.0);
}


void TVectorTest::TranslateTest()
{
	imath::TVector<3, double> vec1 = {1.0, 2.0, 3.0};
	imath::TVector<3, double> vec2 = {4.0, 5.0, 6.0};
	
	// Test Translate
	vec1.Translate(vec2);
	QVERIFY(vec1.GetElement(0) == 5.0);
	QVERIFY(vec1.GetElement(1) == 7.0);
	QVERIFY(vec1.GetElement(2) == 9.0);
	
	// Test GetTranslated
	imath::TVector<3, double> vec3 = {1.0, 2.0, 3.0};
	imath::TVector<3, double> vec4 = vec3.GetTranslated(vec2);
	QVERIFY(vec4.GetElement(0) == 5.0);
	QVERIFY(vec4.GetElement(1) == 7.0);
	QVERIFY(vec4.GetElement(2) == 9.0);
	QVERIFY(vec3.GetElement(0) == 1.0); // Original unchanged
	
	// Test GetTranslated with output parameter
	imath::TVector<3, double> vec5;
	vec3.GetTranslated(vec2, vec5);
	QVERIFY(vec5.GetElement(0) == 5.0);
	QVERIFY(vec5.GetElement(1) == 7.0);
	QVERIFY(vec5.GetElement(2) == 9.0);
}


void TVectorTest::ScaledCumulateTest()
{
	imath::TVector<3, double> vec1 = {1.0, 2.0, 3.0};
	imath::TVector<3, double> vec2 = {4.0, 5.0, 6.0};
	
	// Test ScaledCumulate: vec1 += vec2 * 2.0
	vec1.ScaledCumulate(vec2, 2.0);
	QVERIFY(vec1.GetElement(0) == 9.0);  // 1 + 4*2
	QVERIFY(vec1.GetElement(1) == 12.0); // 2 + 5*2
	QVERIFY(vec1.GetElement(2) == 15.0); // 3 + 6*2
}


void TVectorTest::IsNullTest()
{
	// Test null vector
	imath::TVector<3, double> vec1;
	vec1.Clear();
	QVERIFY(vec1.IsNull());
	
	// Test non-null vector
	imath::TVector<3, double> vec2 = {1.0, 0.0, 0.0};
	QVERIFY(!vec2.IsNull());
	
	// Test with small values (within tolerance)
	imath::TVector<3, double> vec3 = {1e-7, 1e-7, 1e-7};
	QVERIFY(vec3.IsNull(1e-6)); // Should be null with default tolerance
	QVERIFY(!vec3.IsNull(1e-8)); // Should not be null with stricter tolerance
}


void TVectorTest::DotProductTest()
{
	// Test perpendicular vectors (dot product = 0)
	imath::TVector<3, double> vec1 = {1.0, 0.0, 0.0};
	imath::TVector<3, double> vec2 = {0.0, 1.0, 0.0};
	QVERIFY(vec1.GetDotProduct(vec2) == 0.0);
	
	// Test parallel vectors
	imath::TVector<3, double> vec3 = {2.0, 0.0, 0.0};
	QVERIFY(qAbs(vec1.GetDotProduct(vec3) - 2.0) < 1e-10);
	
	// Test general case
	imath::TVector<3, double> vec4 = {1.0, 2.0, 3.0};
	imath::TVector<3, double> vec5 = {4.0, 5.0, 6.0};
	double dotProduct = vec4.GetDotProduct(vec5); // 1*4 + 2*5 + 3*6 = 32
	QVERIFY(qAbs(dotProduct - 32.0) < 1e-10);
}


void TVectorTest::LengthTest()
{
	// Test Length2
	imath::TVector<2, double> vec1 = {3.0, 4.0};
	QVERIFY(qAbs(vec1.GetLength2() - 25.0) < 1e-10); // 3^2 + 4^2 = 25
	
	// Test Length
	QVERIFY(qAbs(vec1.GetLength() - 5.0) < 1e-10); // sqrt(25) = 5
	
	// Test 3D vector
	imath::TVector<3, double> vec2 = {1.0, 2.0, 2.0};
	QVERIFY(qAbs(vec2.GetLength2() - 9.0) < 1e-10); // 1 + 4 + 4 = 9
	QVERIFY(qAbs(vec2.GetLength() - 3.0) < 1e-10); // sqrt(9) = 3
}


void TVectorTest::DistanceTest()
{
	// Test Distance2
	imath::TVector<2, double> point1 = {0.0, 0.0};
	imath::TVector<2, double> point2 = {3.0, 4.0};
	QVERIFY(qAbs(point1.GetDistance2(point2) - 25.0) < 1e-10);
	
	// Test Distance
	QVERIFY(qAbs(point1.GetDistance(point2) - 5.0) < 1e-10);
	
	// Test 3D distance
	imath::TVector<3, double> point3 = {1.0, 2.0, 3.0};
	imath::TVector<3, double> point4 = {4.0, 6.0, 3.0};
	// Distance^2 = (4-1)^2 + (6-2)^2 + (3-3)^2 = 9 + 16 + 0 = 25
	QVERIFY(qAbs(point3.GetDistance2(point4) - 25.0) < 1e-10);
	QVERIFY(qAbs(point3.GetDistance(point4) - 5.0) < 1e-10);
}


void TVectorTest::ElementsSumTest()
{
	imath::TVector<4, double> vec = {1.0, 2.0, 3.0, 4.0};
	QVERIFY(qAbs(vec.GetElementsSum() - 10.0) < 1e-10);
	
	// Test with negative values
	imath::TVector<3, double> vec2 = {-1.0, 2.0, -3.0};
	QVERIFY(qAbs(vec2.GetElementsSum() - (-2.0)) < 1e-10);
}


void TVectorTest::NormalizeTest()
{
	// Test normalization to unit vector
	imath::TVector<2, double> vec1 = {3.0, 4.0};
	bool success = vec1.Normalize();
	QVERIFY(success);
	QVERIFY(qAbs(vec1.GetLength() - 1.0) < 1e-10);
	QVERIFY(qAbs(vec1.GetElement(0) - 0.6) < 1e-10);
	QVERIFY(qAbs(vec1.GetElement(1) - 0.8) < 1e-10);
	
	// Test normalization to specified length
	imath::TVector<2, double> vec2 = {3.0, 4.0};
	success = vec2.Normalize(10.0);
	QVERIFY(success);
	QVERIFY(qAbs(vec2.GetLength() - 10.0) < 1e-10);
	
	// Test normalization of null vector (should fail)
	imath::TVector<2, double> vec3;
	vec3.Clear();
	success = vec3.Normalize();
	QVERIFY(!success);
}


void TVectorTest::GetNormalizedTest()
{
	// Test GetNormalized
	imath::TVector<2, double> vec1 = {3.0, 4.0};
	imath::TVector<2, double> normalized;
	bool success = vec1.GetNormalized(normalized);
	QVERIFY(success);
	QVERIFY(qAbs(normalized.GetLength() - 1.0) < 1e-10);
	QVERIFY(qAbs(vec1.GetLength() - 5.0) < 1e-10); // Original unchanged
	
	// Test with specified length
	imath::TVector<2, double> normalized2;
	success = vec1.GetNormalized(normalized2, 10.0);
	QVERIFY(success);
	QVERIFY(qAbs(normalized2.GetLength() - 10.0) < 1e-10);
	
	// Test with null vector (should fail)
	imath::TVector<2, double> vec2;
	vec2.Clear();
	imath::TVector<2, double> normalized3;
	success = vec2.GetNormalized(normalized3);
	QVERIFY(!success);
}


void TVectorTest::GetMinimalMaximalTest()
{
	imath::TVector<3, double> vec1 = {1.0, 5.0, 3.0};
	imath::TVector<3, double> vec2 = {4.0, 2.0, 6.0};
	imath::TVector<3, double> minimal;
	imath::TVector<3, double> maximal;
	
	// Test GetMinimal
	vec1.GetMinimal(vec2, minimal);
	QVERIFY(minimal.GetElement(0) == 1.0); // min(1, 4)
	QVERIFY(minimal.GetElement(1) == 2.0); // min(5, 2)
	QVERIFY(minimal.GetElement(2) == 3.0); // min(3, 6)
	
	// Test GetMaximal
	vec1.GetMaximal(vec2, maximal);
	QVERIFY(maximal.GetElement(0) == 4.0); // max(1, 4)
	QVERIFY(maximal.GetElement(1) == 5.0); // max(5, 2)
	QVERIFY(maximal.GetElement(2) == 6.0); // max(3, 6)
}


void TVectorTest::cleanupTestCase()
{
}


I_ADD_TEST(TVectorTest);
