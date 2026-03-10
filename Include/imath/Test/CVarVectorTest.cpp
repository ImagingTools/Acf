// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CVarVectorTest.h"


// protected slots

void CVarVectorTest::initTestCase()
{
}


void CVarVectorTest::DefaultConstructorTest()
{
	imath::CVarVector vec;
	QVERIFY(vec.IsEmpty());
	QVERIFY(vec.GetElementsCount() == 0);
}


void CVarVectorTest::SizeConstructorTest()
{
	// Test with size and default value
	imath::CVarVector vec1(3);
	QVERIFY(!vec1.IsEmpty());
	QVERIFY(vec1.GetElementsCount() == 3);
	QVERIFY(vec1.GetElement(0) == 0.0);
	QVERIFY(vec1.GetElement(1) == 0.0);
	QVERIFY(vec1.GetElement(2) == 0.0);
	
	// Test with size and initial value
	imath::CVarVector vec2(4, 5.0);
	QVERIFY(vec2.GetElementsCount() == 4);
	QVERIFY(vec2.GetElement(0) == 5.0);
	QVERIFY(vec2.GetElement(1) == 5.0);
	QVERIFY(vec2.GetElement(2) == 5.0);
	QVERIFY(vec2.GetElement(3) == 5.0);
}


void CVarVectorTest::CopyConstructorTest()
{
	imath::CVarVector vec1(3);
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 2.0);
	vec1.SetElement(2, 3.0);
	
	// Test copy constructor
	imath::CVarVector vec2(vec1);
	QVERIFY(vec2.GetElementsCount() == 3);
	QVERIFY(vec2.GetElement(0) == 1.0);
	QVERIFY(vec2.GetElement(1) == 2.0);
	QVERIFY(vec2.GetElement(2) == 3.0);
}


void CVarVectorTest::TVectorConstructorTest()
{
	// Test constructor from TVector
	imath::TVector<3, double> tvec = {1.0, 2.0, 3.0};
	imath::CVarVector vec(tvec);
	
	QVERIFY(vec.GetElementsCount() == 3);
	QVERIFY(vec.GetElement(0) == 1.0);
	QVERIFY(vec.GetElement(1) == 2.0);
	QVERIFY(vec.GetElement(2) == 3.0);
}


void CVarVectorTest::IsEmptyTest()
{
	imath::CVarVector vec1;
	QVERIFY(vec1.IsEmpty());
	
	imath::CVarVector vec2(3);
	QVERIFY(!vec2.IsEmpty());
	
	vec2.Reset();
	QVERIFY(vec2.IsEmpty());
}


void CVarVectorTest::GetSetElementTest()
{
	imath::CVarVector vec(3);
	
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


void CVarVectorTest::SetAllElementsTest()
{
	imath::CVarVector vec(4);
	vec.SetAllElements(9.0);
	
	QVERIFY(vec.GetElement(0) == 9.0);
	QVERIFY(vec.GetElement(1) == 9.0);
	QVERIFY(vec.GetElement(2) == 9.0);
	QVERIFY(vec.GetElement(3) == 9.0);
}


void CVarVectorTest::ResetClearTest()
{
	// Test Reset() - clears size
	imath::CVarVector vec1(3, 5.0);
	vec1.Reset();
	QVERIFY(vec1.IsEmpty());
	QVERIFY(vec1.GetElementsCount() == 0);
	
	// Test Reset(count, value) - sets size and value
	vec1.Reset(4, 7.0);
	QVERIFY(vec1.GetElementsCount() == 4);
	QVERIFY(vec1.GetElement(0) == 7.0);
	QVERIFY(vec1.GetElement(3) == 7.0);
	
	// Test Clear() - sets elements to zero but keeps size
	imath::CVarVector vec2(3);
	vec2.SetElement(0, 1.0);
	vec2.SetElement(1, 2.0);
	vec2.SetElement(2, 3.0);
	vec2.Clear();
	QVERIFY(vec2.GetElementsCount() == 3);
	QVERIFY(vec2.GetElement(0) == 0.0);
	QVERIFY(vec2.GetElement(1) == 0.0);
	QVERIFY(vec2.GetElement(2) == 0.0);
}


void CVarVectorTest::SetElementsFromTest()
{
	imath::CVarVector vec1(3);
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 2.0);
	vec1.SetElement(2, 3.0);
	
	// Test with same size
	imath::CVarVector vec2(3);
	vec2.SetElementsFrom(vec1);
	QVERIFY(vec2.GetElement(0) == 1.0);
	QVERIFY(vec2.GetElement(1) == 2.0);
	QVERIFY(vec2.GetElement(2) == 3.0);
	
	// Test with larger target (uses expansion value)
	imath::CVarVector vec3(5);
	vec3.SetElementsFrom(vec1, 9.0);
	QVERIFY(vec3.GetElement(0) == 1.0);
	QVERIFY(vec3.GetElement(1) == 2.0);
	QVERIFY(vec3.GetElement(2) == 3.0);
	QVERIFY(vec3.GetElement(3) == 9.0);
	QVERIFY(vec3.GetElement(4) == 9.0);
	
	// Test with smaller target
	imath::CVarVector vec4(2);
	vec4.SetElementsFrom(vec1);
	QVERIFY(vec4.GetElement(0) == 1.0);
	QVERIFY(vec4.GetElement(1) == 2.0);
}


void CVarVectorTest::EnsureElementsCountTest()
{
	imath::CVarVector vec(2);
	vec.SetElement(0, 1.0);
	vec.SetElement(1, 2.0);
	
	// Ensure count doesn't shrink
	bool result = vec.EnsureElementsCount(1);
	QVERIFY(result);
	QVERIFY(vec.GetElementsCount() == 2);
	
	// Ensure count grows
	result = vec.EnsureElementsCount(5, 9.0);
	QVERIFY(result);
	QVERIFY(vec.GetElementsCount() == 5);
	QVERIFY(vec.GetElement(0) == 1.0); // Original values preserved
	QVERIFY(vec.GetElement(1) == 2.0);
	QVERIFY(vec.GetElement(4) == 9.0); // New values use specified value
}


void CVarVectorTest::TranslateTest()
{
	imath::CVarVector vec1(3);
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 2.0);
	vec1.SetElement(2, 3.0);
	
	imath::CVarVector vec2(3);
	vec2.SetElement(0, 4.0);
	vec2.SetElement(1, 5.0);
	vec2.SetElement(2, 6.0);
	
	// Test Translate
	vec1.Translate(vec2);
	QVERIFY(vec1.GetElement(0) == 5.0);
	QVERIFY(vec1.GetElement(1) == 7.0);
	QVERIFY(vec1.GetElement(2) == 9.0);
	
	// Test GetTranslated
	imath::CVarVector vec3(3);
	vec3.SetElement(0, 1.0);
	vec3.SetElement(1, 2.0);
	vec3.SetElement(2, 3.0);
	imath::CVarVector vec4 = vec3.GetTranslated(vec2);
	QVERIFY(vec4.GetElement(0) == 5.0);
	QVERIFY(vec4.GetElement(1) == 7.0);
	QVERIFY(vec4.GetElement(2) == 9.0);
	QVERIFY(vec3.GetElement(0) == 1.0); // Original unchanged
	
	// Test GetTranslated with output parameter
	imath::CVarVector vec5;
	vec3.GetTranslated(vec2, vec5);
	QVERIFY(vec5.GetElement(0) == 5.0);
	QVERIFY(vec5.GetElement(1) == 7.0);
	QVERIFY(vec5.GetElement(2) == 9.0);
}


void CVarVectorTest::ScaledCumulateTest()
{
	imath::CVarVector vec1(3);
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 2.0);
	vec1.SetElement(2, 3.0);
	
	imath::CVarVector vec2(3);
	vec2.SetElement(0, 4.0);
	vec2.SetElement(1, 5.0);
	vec2.SetElement(2, 6.0);
	
	// Test ScaledCumulate: vec1 += vec2 * 2.0
	vec1.ScaledCumulate(vec2, 2.0);
	QVERIFY(vec1.GetElement(0) == 9.0);  // 1 + 4*2
	QVERIFY(vec1.GetElement(1) == 12.0); // 2 + 5*2
	QVERIFY(vec1.GetElement(2) == 15.0); // 3 + 6*2
}


void CVarVectorTest::IsNullTest()
{
	// Test null vector
	imath::CVarVector vec1(3);
	vec1.Clear();
	QVERIFY(vec1.IsNull());
	
	// Test non-null vector
	imath::CVarVector vec2(3);
	vec2.SetElement(0, 1.0);
	vec2.SetElement(1, 0.0);
	vec2.SetElement(2, 0.0);
	QVERIFY(!vec2.IsNull());
	
	// Test with small values (within tolerance)
	imath::CVarVector vec3(3);
	vec3.SetElement(0, 1e-7);
	vec3.SetElement(1, 1e-7);
	vec3.SetElement(2, 1e-7);
	QVERIFY(vec3.IsNull(1e-6)); // Should be null with default tolerance
	QVERIFY(!vec3.IsNull(1e-8)); // Should not be null with stricter tolerance
}


void CVarVectorTest::DotProductTest()
{
	// Test perpendicular vectors (dot product = 0)
	imath::CVarVector vec1(3);
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 0.0);
	vec1.SetElement(2, 0.0);
	
	imath::CVarVector vec2(3);
	vec2.SetElement(0, 0.0);
	vec2.SetElement(1, 1.0);
	vec2.SetElement(2, 0.0);
	
	QVERIFY(vec1.GetDotProduct(vec2) == 0.0);
	
	// Test general case
	imath::CVarVector vec3(3);
	vec3.SetElement(0, 1.0);
	vec3.SetElement(1, 2.0);
	vec3.SetElement(2, 3.0);
	
	imath::CVarVector vec4(3);
	vec4.SetElement(0, 4.0);
	vec4.SetElement(1, 5.0);
	vec4.SetElement(2, 6.0);
	
	double dotProduct = vec3.GetDotProduct(vec4); // 1*4 + 2*5 + 3*6 = 32
	QVERIFY(qAbs(dotProduct - 32.0) < 1e-10);
}


void CVarVectorTest::LengthTest()
{
	// Test Length2
	imath::CVarVector vec1(2);
	vec1.SetElement(0, 3.0);
	vec1.SetElement(1, 4.0);
	QVERIFY(qAbs(vec1.GetLength2() - 25.0) < 1e-10); // 3^2 + 4^2 = 25
	
	// Test Length
	QVERIFY(qAbs(vec1.GetLength() - 5.0) < 1e-10); // sqrt(25) = 5
	
	// Test 3D vector
	imath::CVarVector vec2(3);
	vec2.SetElement(0, 1.0);
	vec2.SetElement(1, 2.0);
	vec2.SetElement(2, 2.0);
	QVERIFY(qAbs(vec2.GetLength2() - 9.0) < 1e-10); // 1 + 4 + 4 = 9
	QVERIFY(qAbs(vec2.GetLength() - 3.0) < 1e-10); // sqrt(9) = 3
}


void CVarVectorTest::DistanceTest()
{
	// Test Distance2
	imath::CVarVector point1(2);
	point1.SetElement(0, 0.0);
	point1.SetElement(1, 0.0);
	
	imath::CVarVector point2(2);
	point2.SetElement(0, 3.0);
	point2.SetElement(1, 4.0);
	
	QVERIFY(qAbs(point1.GetDistance2(point2) - 25.0) < 1e-10);
	
	// Test Distance
	QVERIFY(qAbs(point1.GetDistance(point2) - 5.0) < 1e-10);
}


void CVarVectorTest::ElementsSumTest()
{
	imath::CVarVector vec(4);
	vec.SetElement(0, 1.0);
	vec.SetElement(1, 2.0);
	vec.SetElement(2, 3.0);
	vec.SetElement(3, 4.0);
	QVERIFY(qAbs(vec.GetElementsSum() - 10.0) < 1e-10);
	
	// Test with negative values
	imath::CVarVector vec2(3);
	vec2.SetElement(0, -1.0);
	vec2.SetElement(1, 2.0);
	vec2.SetElement(2, -3.0);
	QVERIFY(qAbs(vec2.GetElementsSum() - (-2.0)) < 1e-10);
}


void CVarVectorTest::NormalizeTest()
{
	// Test normalization to unit vector
	imath::CVarVector vec1(2);
	vec1.SetElement(0, 3.0);
	vec1.SetElement(1, 4.0);
	bool success = vec1.Normalize();
	QVERIFY(success);
	QVERIFY(qAbs(vec1.GetLength() - 1.0) < 1e-10);
	QVERIFY(qAbs(vec1.GetElement(0) - 0.6) < 1e-10);
	QVERIFY(qAbs(vec1.GetElement(1) - 0.8) < 1e-10);
	
	// Test normalization to specified length
	imath::CVarVector vec2(2);
	vec2.SetElement(0, 3.0);
	vec2.SetElement(1, 4.0);
	success = vec2.Normalize(10.0);
	QVERIFY(success);
	QVERIFY(qAbs(vec2.GetLength() - 10.0) < 1e-10);
	
	// Test normalization of null vector (should fail)
	imath::CVarVector vec3(2);
	vec3.Clear();
	success = vec3.Normalize();
	QVERIFY(!success);
}


void CVarVectorTest::GetNormalizedTest()
{
	// Test GetNormalized
	imath::CVarVector vec1(2);
	vec1.SetElement(0, 3.0);
	vec1.SetElement(1, 4.0);
	imath::CVarVector normalized;
	bool success = vec1.GetNormalized(normalized);
	QVERIFY(success);
	QVERIFY(qAbs(normalized.GetLength() - 1.0) < 1e-10);
	QVERIFY(qAbs(vec1.GetLength() - 5.0) < 1e-10); // Original unchanged
	
	// Test with specified length
	imath::CVarVector normalized2;
	success = vec1.GetNormalized(normalized2, 10.0);
	QVERIFY(success);
	QVERIFY(qAbs(normalized2.GetLength() - 10.0) < 1e-10);
	
	// Test with null vector (should fail)
	imath::CVarVector vec2(2);
	vec2.Clear();
	imath::CVarVector normalized3;
	success = vec2.GetNormalized(normalized3);
	QVERIFY(!success);
}


void CVarVectorTest::cleanupTestCase()
{
}


I_ADD_TEST(CVarVectorTest);
