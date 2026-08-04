// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CFastColorTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <icmm/CFastColor.h>
#include <icmm/CLab.h>
#include <icmm/CVarColor.h>


void CFastColorTest::ConstructorTest()
{
	// Default constructor creates an empty color
	icmm::CFastColor empty;
	QCOMPARE(empty.GetElementsCount(), 0);

	// Constructor with explicit count (zero initialized)
	icmm::CFastColor zeros(3);
	QCOMPARE(zeros.GetElementsCount(), 3);
	for (int i = 0; i < 3; ++i){
		QCOMPARE(zeros.GetElement(i), 0.0);
	}

	// Constructor with explicit fill value
	icmm::CFastColor filled(4, 0.75);
	QCOMPARE(filled.GetElementsCount(), 4);
	for (int i = 0; i < 4; ++i){
		QCOMPARE(filled.GetElement(i), 0.75);
	}

	// Copy constructor
	icmm::CFastColor copy(filled);
	QVERIFY(copy == filled);

	// Conversion from CVarColor
	icmm::CVarColor varColor(3);
	varColor.SetElement(0, 0.1);
	varColor.SetElement(1, 0.2);
	varColor.SetElement(2, 0.3);
	icmm::CFastColor converted(varColor);
	QCOMPARE(converted.GetElementsCount(), 3);
	QCOMPARE(converted.GetElement(2), 0.3);

	// Conversion back to CVarColor
	icmm::CVarColor roundTrip = converted;
	QCOMPARE(roundTrip.GetElementsCount(), 3);
	QCOMPARE(roundTrip.GetElement(1), 0.2);
}


void CFastColorTest::InitializerListTest()
{
	icmm::CFastColor color{0.1, 0.2, 0.3, 0.4};
	QCOMPARE(color.GetElementsCount(), 4);
	QCOMPARE(color.GetElement(0), 0.1);
	QCOMPARE(color.GetElement(3), 0.4);

	icmm::CFastColor empty{};
	QCOMPARE(empty.GetElementsCount(), 0);
}


void CFastColorTest::EnsureElementsCountTest()
{
	icmm::CFastColor color(2, 0.5);

	// Growing appends elements with the given default value
	QVERIFY(color.EnsureElementsCount(4, 0.25));
	QCOMPARE(color.GetElementsCount(), 4);
	QCOMPARE(color.GetElement(0), 0.5);
	QCOMPARE(color.GetElement(3), 0.25);

	// A smaller count must not shrink the color
	QVERIFY(color.EnsureElementsCount(2));
	QCOMPARE(color.GetElementsCount(), 4);

	// Same count is a no-op
	QVERIFY(color.EnsureElementsCount(4));
	QCOMPARE(color.GetElementsCount(), 4);
}


void CFastColorTest::LabConversionTest()
{
	// SetAsLab/GetAsLab round trip
	icmm::CLab lab(50.0, -20.0, 30.0);

	icmm::CFastColor color;
	color.SetAsLab(lab);
	QCOMPARE(color.GetElementsCount(), 3);

	icmm::CLab restored = color.GetAsLab();
	QVERIFY(qAbs(restored.GetL() - 50.0) < 0.001);
	QVERIFY(qAbs(restored.GetA() + 20.0) < 0.001);
	QVERIFY(qAbs(restored.GetB() - 30.0) < 0.001);

	// A color with less than 3 components converts to neutral Lab
	icmm::CFastColor tooSmall(2, 0.7);
	icmm::CLab neutral = tooSmall.GetAsLab();
	QCOMPARE(neutral.GetL(), 0.0);
	QCOMPARE(neutral.GetA(), 0.0);
	QCOMPARE(neutral.GetB(), 0.0);
}


void CFastColorTest::IsSimilarTest()
{
	icmm::CFastColor color1{0.5, 0.5, 0.5};
	icmm::CFastColor color2{0.501, 0.499, 0.502};

	QVERIFY(color1.IsSimilar(color1));
	QVERIFY(color1.IsSimilar(color2, 0.01));
	QVERIFY(!color1.IsSimilar(color2, 0.0001));
}


void CFastColorTest::NormalizeTest()
{
	icmm::CFastColor color{-0.5, 0.5, 1.5};
	QVERIFY(!color.IsNormalized());

	color.Normalize();
	QVERIFY(color.IsNormalized());
	QCOMPARE(color.GetElement(0), 0.0);
	QCOMPARE(color.GetElement(1), 0.5);
	QCOMPARE(color.GetElement(2), 1.0);

	// GetNormalized() must not modify the source object
	icmm::CFastColor hdr{2.0, -1.0};
	icmm::CFastColor normalized;
	hdr.GetNormalized(normalized);
	QCOMPARE(normalized.GetElement(0), 1.0);
	QCOMPARE(normalized.GetElement(1), 0.0);
	QCOMPARE(hdr.GetElement(0), 2.0);
}


void CFastColorTest::ScalarOperatorsTest()
{
	icmm::CFastColor color{0.2, 0.4, 0.8};

	icmm::CFastColor scaled = color * 2.0;
	QVERIFY(qAbs(scaled.GetElement(0) - 0.4) < 0.001);
	QVERIFY(qAbs(scaled.GetElement(1) - 0.8) < 0.001);
	QVERIFY(qAbs(scaled.GetElement(2) - 1.6) < 0.001);

	icmm::CFastColor divided = color / 2.0;
	QVERIFY(qAbs(divided.GetElement(0) - 0.1) < 0.001);
	QVERIFY(qAbs(divided.GetElement(1) - 0.2) < 0.001);
	QVERIFY(qAbs(divided.GetElement(2) - 0.4) < 0.001);

	icmm::CFastColor compound = color;
	compound *= 2.0;
	QVERIFY(compound.IsSimilar(scaled, 0.0001));

	compound = color;
	compound /= 2.0;
	QVERIFY(compound.IsSimilar(divided, 0.0001));
}


void CFastColorTest::ComparisonOperatorsTest()
{
	icmm::CFastColor color1{0.1, 0.2, 0.3};
	icmm::CFastColor color2{0.1, 0.2, 0.3};
	icmm::CFastColor color3{0.1, 0.2, 0.4};

	QVERIFY(color1 == color2);
	QVERIFY(!(color1 == color3));
	QVERIFY(color1 != color3);

	QVERIFY(color1 < color3);
	QVERIFY(color3 > color1);
	QVERIFY(color1 <= color2);
	QVERIFY(color1 >= color2);

	// Colors of different sizes are not equal
	icmm::CFastColor shorter{0.1, 0.2};
	QVERIFY(color1 != shorter);
}


I_ADD_TEST(CFastColorTest);
