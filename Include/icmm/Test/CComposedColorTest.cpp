// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CComposedColorTest.h>


// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CCmy.h>
#include <icmm/CCmyk.h>
#include <icmm/CLab.h>
#include <icmm/CRgb.h>


namespace
{

template<class T>
void TestSerializationRoundTrip(T source)
{
	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(source.Serialize(writeArchive));

	iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
	T restored;
	QVERIFY(restored.Serialize(readArchive));
	QVERIFY(restored == source);
}

}


// CRgb

void CComposedColorTest::RgbConstructorTest()
{
	// Default constructor creates black
	icmm::CRgb black;
	QCOMPARE(black.GetRed(), 0.0);
	QCOMPARE(black.GetGreen(), 0.0);
	QCOMPARE(black.GetBlue(), 0.0);

	// Partial argument list defaults remaining components to 0
	icmm::CRgb red(1.0);
	QCOMPARE(red.GetRed(), 1.0);
	QCOMPARE(red.GetGreen(), 0.0);
	QCOMPARE(red.GetBlue(), 0.0);

	// Full constructor and copy constructor
	icmm::CRgb color(0.2, 0.4, 0.6);
	icmm::CRgb copy(color);
	QCOMPARE(copy.GetRed(), 0.2);
	QCOMPARE(copy.GetGreen(), 0.4);
	QCOMPARE(copy.GetBlue(), 0.6);
	QVERIFY(copy == color);

	QCOMPARE(icmm::CRgb::GetElementsCount(), 3);
}


void CComposedColorTest::RgbGetSetTest()
{
	icmm::CRgb color;
	color.SetRed(0.8);
	color.SetGreen(0.3);
	color.SetBlue(0.1);

	QCOMPARE(color.GetRed(), 0.8);
	QCOMPARE(color.GetGreen(), 0.3);
	QCOMPARE(color.GetBlue(), 0.1);

	QCOMPARE(color.GetElement(icmm::CRgb::CI_RED), 0.8);
	QCOMPARE(color.GetElement(icmm::CRgb::CI_GREEN), 0.3);
	QCOMPARE(color.GetElement(icmm::CRgb::CI_BLUE), 0.1);
}


void CComposedColorTest::RgbOperatorsTest()
{
	icmm::CRgb color1(0.8, 0.3, 0.2);
	icmm::CRgb color2(0.2, 0.5, 0.4);

	icmm::CRgb sum = color1 + color2;
	QVERIFY(qAbs(sum.GetRed() - 1.0) < 0.001);
	QVERIFY(qAbs(sum.GetGreen() - 0.8) < 0.001);
	QVERIFY(qAbs(sum.GetBlue() - 0.6) < 0.001);

	icmm::CRgb diff = color1 - color2;
	QVERIFY(qAbs(diff.GetRed() - 0.6) < 0.001);
	QVERIFY(qAbs(diff.GetGreen() + 0.2) < 0.001);
	QVERIFY(qAbs(diff.GetBlue() + 0.2) < 0.001);

	icmm::CRgb prod = color1 * color2;
	QVERIFY(qAbs(prod.GetRed() - 0.16) < 0.001);
	QVERIFY(qAbs(prod.GetGreen() - 0.15) < 0.001);
	QVERIFY(qAbs(prod.GetBlue() - 0.08) < 0.001);

	icmm::CRgb quot = color1 / color2;
	QVERIFY(qAbs(quot.GetRed() - 4.0) < 0.001);
	QVERIFY(qAbs(quot.GetGreen() - 0.6) < 0.001);
	QVERIFY(qAbs(quot.GetBlue() - 0.5) < 0.001);

	icmm::CRgb scaled = color1 * 0.5;
	QVERIFY(qAbs(scaled.GetRed() - 0.4) < 0.001);

	icmm::CRgb divided = color1 / 2.0;
	QVERIFY(qAbs(divided.GetRed() - 0.4) < 0.001);

	// Compound operators
	icmm::CRgb compound = color1;
	compound += color2;
	QVERIFY(compound.IsSimilar(sum, 0.0001));

	compound = color1;
	compound -= color2;
	QVERIFY(compound.IsSimilar(diff, 0.0001));

	compound = color1;
	compound *= color2;
	QVERIFY(compound.IsSimilar(prod, 0.0001));

	compound = color1;
	compound /= color2;
	QVERIFY(compound.IsSimilar(quot, 0.0001));

	compound = color1;
	compound *= 0.5;
	QVERIFY(compound.IsSimilar(scaled, 0.0001));

	compound = color1;
	compound /= 2.0;
	QVERIFY(compound.IsSimilar(divided, 0.0001));
}


void CComposedColorTest::RgbNormalizeTest()
{
	// HDR values are clamped to [0, 1]
	icmm::CRgb hdr(1.5, -0.2, 0.5);
	QVERIFY(!hdr.IsNormalized());

	icmm::CRgb normalized;
	hdr.GetNormalized(normalized);
	QCOMPARE(normalized.GetRed(), 1.0);
	QCOMPARE(normalized.GetGreen(), 0.0);
	QCOMPARE(normalized.GetBlue(), 0.5);

	// Source is unchanged
	QCOMPARE(hdr.GetRed(), 1.5);

	hdr.Normalize();
	QVERIFY(hdr.IsNormalized());
	QVERIFY(hdr == normalized);

	// Boundary values are normalized
	icmm::CRgb boundary(0.0, 1.0, 0.5);
	QVERIFY(boundary.IsNormalized());
}


void CComposedColorTest::RgbSerializeEdgeCasesTest()
{
	TestSerializationRoundTrip(icmm::CRgb());
	TestSerializationRoundTrip(icmm::CRgb(0.0, 0.0, 0.0));
	TestSerializationRoundTrip(icmm::CRgb(1.0, 1.0, 1.0));
	TestSerializationRoundTrip(icmm::CRgb(-1.5, 0.5, 2.5));
	TestSerializationRoundTrip(icmm::CRgb(1e-12, 1e12, -1e-12));
}


// CCmy

void CComposedColorTest::CmyConstructorTest()
{
	icmm::CCmy defaultColor;
	QCOMPARE(defaultColor.GetC(), 0.0);
	QCOMPARE(defaultColor.GetM(), 0.0);
	QCOMPARE(defaultColor.GetY(), 0.0);

	icmm::CCmy color(0.1, 0.2, 0.3);
	icmm::CCmy copy(color);
	QCOMPARE(copy.GetC(), 0.1);
	QCOMPARE(copy.GetM(), 0.2);
	QCOMPARE(copy.GetY(), 0.3);
	QVERIFY(copy == color);

	QCOMPARE(icmm::CCmy::GetElementsCount(), 3);
}


void CComposedColorTest::CmyGetSetTest()
{
	icmm::CCmy color;
	color.SetC(0.4);
	color.SetM(0.5);
	color.SetY(0.6);

	QCOMPARE(color.GetC(), 0.4);
	QCOMPARE(color.GetM(), 0.5);
	QCOMPARE(color.GetY(), 0.6);
}


void CComposedColorTest::CmyOperatorsTest()
{
	icmm::CCmy color1(0.4, 0.6, 0.8);
	icmm::CCmy color2(0.2, 0.3, 0.4);

	icmm::CCmy sum = color1 + color2;
	QVERIFY(qAbs(sum.GetC() - 0.6) < 0.001);
	QVERIFY(qAbs(sum.GetM() - 0.9) < 0.001);
	QVERIFY(qAbs(sum.GetY() - 1.2) < 0.001);

	icmm::CCmy diff = color1 - color2;
	QVERIFY(qAbs(diff.GetC() - 0.2) < 0.001);
	QVERIFY(qAbs(diff.GetM() - 0.3) < 0.001);
	QVERIFY(qAbs(diff.GetY() - 0.4) < 0.001);

	icmm::CCmy quot = color1 / color2;
	QVERIFY(qAbs(quot.GetC() - 2.0) < 0.001);
	QVERIFY(qAbs(quot.GetM() - 2.0) < 0.001);
	QVERIFY(qAbs(quot.GetY() - 2.0) < 0.001);

	icmm::CCmy assigned;
	assigned = color1;
	QVERIFY(assigned == color1);

	icmm::CCmy compound = color1;
	compound += color2;
	QVERIFY(compound.IsSimilar(sum, 0.0001));

	compound = color1;
	compound *= 2.0;
	QVERIFY(qAbs(compound.GetC() - 0.8) < 0.001);
}


// CCmyk

void CComposedColorTest::CmykConstructorTest()
{
	icmm::CCmyk defaultColor;
	QCOMPARE(defaultColor.GetC(), 0.0);
	QCOMPARE(defaultColor.GetM(), 0.0);
	QCOMPARE(defaultColor.GetY(), 0.0);
	QCOMPARE(defaultColor.GetK(), 0.0);

	icmm::CCmyk color(0.1, 0.2, 0.3, 0.4);
	icmm::CCmyk copy(color);
	QCOMPARE(copy.GetC(), 0.1);
	QCOMPARE(copy.GetM(), 0.2);
	QCOMPARE(copy.GetY(), 0.3);
	QCOMPARE(copy.GetK(), 0.4);
	QVERIFY(copy == color);

	QCOMPARE(icmm::CCmyk::GetElementsCount(), 4);
}


void CComposedColorTest::CmykGetSetTest()
{
	icmm::CCmyk color;
	color.SetC(0.2);
	color.SetM(0.5);
	color.SetY(0.8);
	color.SetK(0.1);

	QCOMPARE(color.GetC(), 0.2);
	QCOMPARE(color.GetM(), 0.5);
	QCOMPARE(color.GetY(), 0.8);
	QCOMPARE(color.GetK(), 0.1);

	QCOMPARE(color.GetElement(icmm::CCmyk::CI_BLACK), 0.1);
}


void CComposedColorTest::CmykOperatorsTest()
{
	icmm::CCmyk color1(0.4, 0.6, 0.8, 0.2);
	icmm::CCmyk color2(0.2, 0.3, 0.4, 0.1);

	icmm::CCmyk sum = color1 + color2;
	QVERIFY(qAbs(sum.GetC() - 0.6) < 0.001);
	QVERIFY(qAbs(sum.GetM() - 0.9) < 0.001);
	QVERIFY(qAbs(sum.GetY() - 1.2) < 0.001);
	QVERIFY(qAbs(sum.GetK() - 0.3) < 0.001);

	icmm::CCmyk diff = color1 - color2;
	QVERIFY(qAbs(diff.GetK() - 0.1) < 0.001);

	icmm::CCmyk scaled = color1 * 0.5;
	QVERIFY(qAbs(scaled.GetC() - 0.2) < 0.001);
	QVERIFY(qAbs(scaled.GetK() - 0.1) < 0.001);

	icmm::CCmyk compound = color1;
	compound /= color2;
	QVERIFY(qAbs(compound.GetC() - 2.0) < 0.001);
	QVERIFY(qAbs(compound.GetK() - 2.0) < 0.001);
}


// CLab

void CComposedColorTest::LabConstructorTest()
{
	icmm::CLab defaultColor;
	QCOMPARE(defaultColor.GetL(), 0.0);
	QCOMPARE(defaultColor.GetA(), 0.0);
	QCOMPARE(defaultColor.GetB(), 0.0);

	// Lab components can be negative
	icmm::CLab color(53.0, -80.0, 67.0);
	icmm::CLab copy(color);
	QCOMPARE(copy.GetL(), 53.0);
	QCOMPARE(copy.GetA(), -80.0);
	QCOMPARE(copy.GetB(), 67.0);
	QVERIFY(copy == color);
}


void CComposedColorTest::LabGetSetTest()
{
	icmm::CLab color;
	color.SetL(75.0);
	color.SetA(25.0);
	color.SetB(-30.0);

	QCOMPARE(color.GetL(), 75.0);
	QCOMPARE(color.GetA(), 25.0);
	QCOMPARE(color.GetB(), -30.0);

	QCOMPARE(color.GetElement(icmm::CLab::CI_L), 75.0);
	QCOMPARE(color.GetElement(icmm::CLab::CI_A), 25.0);
	QCOMPARE(color.GetElement(icmm::CLab::CI_B), -30.0);
}


void CComposedColorTest::LabDeltaETest()
{
	// DeltaE of identical colors is 0
	icmm::CLab color(50.0, 10.0, -10.0);
	QCOMPARE(color.GetDeltaE(color), 0.0);

	// Known Euclidean distance (3-4-5 triangle)
	icmm::CLab color1(50.0, 0.0, 0.0);
	icmm::CLab color2(50.0, 3.0, 4.0);
	QVERIFY(qAbs(color1.GetDeltaE(color2) - 5.0) < 0.001);

	// DeltaE is symmetric
	QVERIFY(qAbs(color1.GetDeltaE(color2) - color2.GetDeltaE(color1)) < 0.001);

	// Distance along the lightness axis only
	icmm::CLab black(0.0, 0.0, 0.0);
	icmm::CLab white(100.0, 0.0, 0.0);
	QVERIFY(qAbs(black.GetDeltaE(white) - 100.0) < 0.001);
}


void CComposedColorTest::LabSerializeEdgeCasesTest()
{
	TestSerializationRoundTrip(icmm::CLab());
	TestSerializationRoundTrip(icmm::CLab(100.0, 0.0, 0.0));
	TestSerializationRoundTrip(icmm::CLab(53.0, -80.0, 67.0));
	TestSerializationRoundTrip(icmm::CLab(-16.0, 127.0, -128.0));
}


// common behavior

void CComposedColorTest::IsSimilarTest()
{
	icmm::CRgb color1(0.5, 0.5, 0.5);
	icmm::CRgb color2(0.501, 0.499, 0.502);

	QVERIFY(color1.IsSimilar(color1));
	QVERIFY(color1.IsSimilar(color2, 0.01));
	QVERIFY(!color1.IsSimilar(color2, 0.0001));
}


void CComposedColorTest::AssignScalarTest()
{
	// Scalar assignment sets the first component and resets the others
	icmm::TComposedColor<3> color;
	color.SetElement(0, 0.1);
	color.SetElement(1, 0.2);
	color.SetElement(2, 0.3);
	color = 0.5;
	QCOMPARE(color.GetElement(0), 0.5);
	QCOMPARE(color.GetElement(1), 0.0);
	QCOMPARE(color.GetElement(2), 0.0);
}


void CComposedColorTest::VarVectorConversionTest()
{
	imath::CVarVector vector(3);
	vector.SetElement(0, 0.7);
	vector.SetElement(1, 0.8);
	vector.SetElement(2, 0.9);

	icmm::TComposedColor<3> color(vector);
	QCOMPARE(color.GetElement(0), 0.7);
	QCOMPARE(color.GetElement(1), 0.8);
	QCOMPARE(color.GetElement(2), 0.9);
}


I_ADD_TEST(CComposedColorTest);
