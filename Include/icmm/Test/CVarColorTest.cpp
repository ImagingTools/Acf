// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CVarColorTest.h>


// ACF includes
#include <imath/CDoubleManip.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CVarColor.h>


void CVarColorTest::ConstructorTest()
{
	// Default constructor creates an empty color
	icmm::CVarColor empty;
	QCOMPARE(empty.GetElementsCount(), 0);

	// Constructor with explicit component count (zero initialized)
	icmm::CVarColor zeros(3);
	QCOMPARE(zeros.GetElementsCount(), 3);
	for (int i = 0; i < 3; ++i){
		QCOMPARE(zeros.GetElement(i), 0.0);
	}

	// Constructor with explicit fill value
	icmm::CVarColor filled(4, 0.25);
	QCOMPARE(filled.GetElementsCount(), 4);
	for (int i = 0; i < 4; ++i){
		QCOMPARE(filled.GetElement(i), 0.25);
	}

	// Copy constructor
	icmm::CVarColor copy(filled);
	QCOMPARE(copy.GetElementsCount(), 4);
	QVERIFY(copy == filled);

	// Conversion constructor from fixed-size vector
	imath::TVector<3> vector;
	vector.SetElement(0, 0.1);
	vector.SetElement(1, 0.2);
	vector.SetElement(2, 0.3);
	icmm::CVarColor converted(vector);
	QCOMPARE(converted.GetElementsCount(), 3);
	QCOMPARE(converted.GetElement(1), 0.2);
}


void CVarColorTest::AssignmentTest()
{
	icmm::CVarColor source(3, 0.5);
	icmm::CVarColor target(3);

	target = source;
	QVERIFY(target == source);

	// Self assignment must be safe
	target = target;
	QVERIFY(target == source);
}


void CVarColorTest::ElementAccessTest()
{
	icmm::CVarColor color(3);
	color.SetElement(0, -1.5);
	color.SetElement(1, 0.0);
	color.SetElement(2, 2.5);

	QCOMPARE(color.GetElement(0), -1.5);
	QCOMPARE(color.GetElement(1), 0.0);
	QCOMPARE(color.GetElement(2), 2.5);

	QCOMPARE(color[0], -1.5);
	QCOMPARE(color[2], 2.5);
}


void CVarColorTest::IsSimilarTest()
{
	icmm::CVarColor color1(3);
	color1.SetElement(0, 0.5);
	color1.SetElement(1, 0.5);
	color1.SetElement(2, 0.5);

	// A color is always similar to itself
	QVERIFY(color1.IsSimilar(color1));

	icmm::CVarColor color2(3);
	color2.SetElement(0, 0.501);
	color2.SetElement(1, 0.499);
	color2.SetElement(2, 0.502);

	// Within tolerance
	QVERIFY(color1.IsSimilar(color2, 0.01));

	// Outside tolerance
	QVERIFY(!color1.IsSimilar(color2, 0.0001));
}


void CVarColorTest::NormalizeTest()
{
	// Values outside [0, 1] are clamped
	icmm::CVarColor color(3);
	color.SetElement(0, -0.5);
	color.SetElement(1, 0.5);
	color.SetElement(2, 1.5);

	QVERIFY(!color.IsNormalized());

	color.Normalize();
	QVERIFY(color.IsNormalized());
	QCOMPARE(color.GetElement(0), 0.0);
	QCOMPARE(color.GetElement(1), 0.5);
	QCOMPARE(color.GetElement(2), 1.0);

	// Boundary values 0 and 1 are already normalized
	icmm::CVarColor boundary(2);
	boundary.SetElement(0, 0.0);
	boundary.SetElement(1, 1.0);
	QVERIFY(boundary.IsNormalized());

	// GetNormalized() must not modify the source object
	icmm::CVarColor hdr(2);
	hdr.SetElement(0, 2.0);
	hdr.SetElement(1, -1.0);

	icmm::CVarColor normalized;
	hdr.GetNormalized(normalized);
	QCOMPARE(normalized.GetElement(0), 1.0);
	QCOMPARE(normalized.GetElement(1), 0.0);
	QCOMPARE(hdr.GetElement(0), 2.0);
	QCOMPARE(hdr.GetElement(1), -1.0);

	// An empty color is trivially normalized
	icmm::CVarColor empty;
	QVERIFY(empty.IsNormalized());
	empty.Normalize();
	QCOMPARE(empty.GetElementsCount(), 0);
}


void CVarColorTest::ArithmeticOperatorsTest()
{
	icmm::CVarColor color1(3);
	color1.SetElement(0, 0.8);
	color1.SetElement(1, 0.3);
	color1.SetElement(2, 0.2);

	icmm::CVarColor color2(3);
	color2.SetElement(0, 0.2);
	color2.SetElement(1, 0.5);
	color2.SetElement(2, 0.1);

	icmm::CVarColor sum = color1 + color2;
	QVERIFY(qAbs(sum.GetElement(0) - 1.0) < 0.001);
	QVERIFY(qAbs(sum.GetElement(1) - 0.8) < 0.001);
	QVERIFY(qAbs(sum.GetElement(2) - 0.3) < 0.001);

	icmm::CVarColor diff = color1 - color2;
	QVERIFY(qAbs(diff.GetElement(0) - 0.6) < 0.001);
	QVERIFY(qAbs(diff.GetElement(1) + 0.2) < 0.001);
	QVERIFY(qAbs(diff.GetElement(2) - 0.1) < 0.001);

	icmm::CVarColor prod = color1 * color2;
	QVERIFY(qAbs(prod.GetElement(0) - 0.16) < 0.001);
	QVERIFY(qAbs(prod.GetElement(1) - 0.15) < 0.001);
	QVERIFY(qAbs(prod.GetElement(2) - 0.02) < 0.001);

	icmm::CVarColor quot = color1 / color2;
	QVERIFY(qAbs(quot.GetElement(0) - 4.0) < 0.001);
	QVERIFY(qAbs(quot.GetElement(1) - 0.6) < 0.001);
	QVERIFY(qAbs(quot.GetElement(2) - 2.0) < 0.001);

	icmm::CVarColor scaled = color1 * 2.0;
	QVERIFY(qAbs(scaled.GetElement(0) - 1.6) < 0.001);
	QVERIFY(qAbs(scaled.GetElement(1) - 0.6) < 0.001);
	QVERIFY(qAbs(scaled.GetElement(2) - 0.4) < 0.001);

	icmm::CVarColor divided = color1 / 2.0;
	QVERIFY(qAbs(divided.GetElement(0) - 0.4) < 0.001);
	QVERIFY(qAbs(divided.GetElement(1) - 0.15) < 0.001);
	QVERIFY(qAbs(divided.GetElement(2) - 0.1) < 0.001);
}


void CVarColorTest::CompoundOperatorsTest()
{
	icmm::CVarColor base(2);
	base.SetElement(0, 0.4);
	base.SetElement(1, 0.6);

	icmm::CVarColor other(2);
	other.SetElement(0, 0.2);
	other.SetElement(1, 0.3);

	icmm::CVarColor color = base;
	color += other;
	QVERIFY(qAbs(color.GetElement(0) - 0.6) < 0.001);
	QVERIFY(qAbs(color.GetElement(1) - 0.9) < 0.001);

	color = base;
	color -= other;
	QVERIFY(qAbs(color.GetElement(0) - 0.2) < 0.001);
	QVERIFY(qAbs(color.GetElement(1) - 0.3) < 0.001);

	color = base;
	color *= other;
	QVERIFY(qAbs(color.GetElement(0) - 0.08) < 0.001);
	QVERIFY(qAbs(color.GetElement(1) - 0.18) < 0.001);

	color = base;
	color /= other;
	QVERIFY(qAbs(color.GetElement(0) - 2.0) < 0.001);
	QVERIFY(qAbs(color.GetElement(1) - 2.0) < 0.001);

	color = base;
	color *= 3.0;
	QVERIFY(qAbs(color.GetElement(0) - 1.2) < 0.001);
	QVERIFY(qAbs(color.GetElement(1) - 1.8) < 0.001);

	color = base;
	color /= 2.0;
	QVERIFY(qAbs(color.GetElement(0) - 0.2) < 0.001);
	QVERIFY(qAbs(color.GetElement(1) - 0.3) < 0.001);
}


void CVarColorTest::MixedSizeOperationsTest()
{
	// Compound operations use the common component count only
	icmm::CVarColor color3(3, 0.5);
	icmm::CVarColor color2(2, 0.25);

	color3 += color2;
	QCOMPARE(color3.GetElementsCount(), 3);
	QVERIFY(qAbs(color3.GetElement(0) - 0.75) < 0.001);
	QVERIFY(qAbs(color3.GetElement(1) - 0.75) < 0.001);
	QVERIFY(qAbs(color3.GetElement(2) - 0.5) < 0.001);
}


void CVarColorTest::RoundingTest()
{
	imath::CDoubleManip manipulator;

	icmm::CVarColor color(2);
	color.SetElement(0, 0.123456789);
	color.SetElement(1, 0.987654321);

	icmm::CVarColor rounded;
	color.GetRounded(manipulator, rounded);
	QCOMPARE(rounded.GetElementsCount(), 2);

	QVERIFY(color.IsRoundedEqual(rounded, manipulator));
	QVERIFY(color.IsRoundedEqual(color, manipulator));
}


void CVarColorTest::SerializeTest()
{
	// Round trip with values outside the normalized range
	icmm::CVarColor source(4);
	source.SetElement(0, -1.25);
	source.SetElement(1, 0.0);
	source.SetElement(2, 0.5);
	source.SetElement(3, 100.75);

	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(source.Serialize(writeArchive));

	// Restored object must adapt its component count
	icmm::CVarColor restored;
	iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
	QVERIFY(restored.Serialize(readArchive));
	QCOMPARE(restored.GetElementsCount(), 4);
	QVERIFY(restored == source);

	// Round trip of an empty color
	icmm::CVarColor emptySource;
	iser::CJsonMemWriteArchive emptyWriteArchive;
	QVERIFY(emptySource.Serialize(emptyWriteArchive));

	icmm::CVarColor emptyRestored(2, 1.0);
	iser::CJsonMemReadArchive emptyReadArchive(emptyWriteArchive.GetData());
	QVERIFY(emptyRestored.Serialize(emptyReadArchive));
	QCOMPARE(emptyRestored.GetElementsCount(), 0);
}


I_ADD_TEST(CVarColorTest);
