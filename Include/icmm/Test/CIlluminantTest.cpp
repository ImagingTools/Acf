// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CIlluminantTest.h>


// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CIlluminant.h>


namespace
{

icmm::CVarColor MakeWhitePoint(double x, double y, double z)
{
	icmm::CVarColor whitePoint(3);
	whitePoint.SetElement(0, x);
	whitePoint.SetElement(1, y);
	whitePoint.SetElement(2, z);

	return whitePoint;
}

}


void CIlluminantTest::StandardIlluminantConstructorTest()
{
	// Default constructor creates a D50 illuminant
	icmm::CIlluminant defaultIlluminant;
	QCOMPARE(defaultIlluminant.GetIlluminantType(), icmm::StandardIlluminant::D50);
	QCOMPARE(defaultIlluminant.GetIlluminantName(), QString("D50"));

	// The illuminant name is derived from the standard type
	struct
	{
		icmm::StandardIlluminant type;
		QString name;
	} testCases[] = {
		{icmm::StandardIlluminant::A, "A"},
		{icmm::StandardIlluminant::B, "B"},
		{icmm::StandardIlluminant::C, "C"},
		{icmm::StandardIlluminant::D50, "D50"},
		{icmm::StandardIlluminant::D55, "D55"},
		{icmm::StandardIlluminant::D60, "D60"},
		{icmm::StandardIlluminant::D65, "D65"},
		{icmm::StandardIlluminant::D75, "D75"},
		{icmm::StandardIlluminant::E, "E"}};

	for (const auto& testCase: testCases){
		icmm::CIlluminant illuminant(testCase.type);
		QCOMPARE(illuminant.GetIlluminantType(), testCase.type);
		QCOMPARE(illuminant.GetIlluminantName(), testCase.name);
	}
}


void CIlluminantTest::CustomIlluminantConstructorTest()
{
	icmm::CVarColor whitePoint = MakeWhitePoint(0.9505, 1.0, 1.089);

	icmm::CIlluminant illuminant("MyIlluminant", whitePoint);
	QCOMPARE(illuminant.GetIlluminantType(), icmm::StandardIlluminant::Custom);
	QCOMPARE(illuminant.GetIlluminantName(), QString("MyIlluminant"));
	QVERIFY(illuminant.GetWhitePoint() == whitePoint);

	// Custom illuminant with an empty name and empty white point
	icmm::CIlluminant emptyIlluminant{QString(), icmm::CVarColor()};
	QCOMPARE(emptyIlluminant.GetIlluminantType(), icmm::StandardIlluminant::Custom);
	QVERIFY(emptyIlluminant.GetIlluminantName().isEmpty());
	QCOMPARE(emptyIlluminant.GetWhitePoint().GetElementsCount(), 0);
}


void CIlluminantTest::CopyConstructorTest()
{
	icmm::CIlluminant source("Source", MakeWhitePoint(0.5, 0.6, 0.7));

	icmm::CIlluminant copy(source);
	QCOMPARE(copy.GetIlluminantType(), source.GetIlluminantType());
	QCOMPARE(copy.GetIlluminantName(), source.GetIlluminantName());
	QVERIFY(copy.GetWhitePoint() == source.GetWhitePoint());
	QVERIFY(copy.IsEqual(source));
}


void CIlluminantTest::SettersTest()
{
	icmm::CIlluminant illuminant;

	illuminant.SetIlluminantType(icmm::StandardIlluminant::D65);
	QCOMPARE(illuminant.GetIlluminantType(), icmm::StandardIlluminant::D65);

	illuminant.SetIlluminantName("NewName");
	QCOMPARE(illuminant.GetIlluminantName(), QString("NewName"));

	icmm::CVarColor whitePoint = MakeWhitePoint(0.1, 0.2, 0.3);
	illuminant.SetWhitePoint(whitePoint);
	QVERIFY(illuminant.GetWhitePoint() == whitePoint);
}


void CIlluminantTest::FactoryIdTest()
{
	icmm::CIlluminant illuminant;
	QCOMPARE(illuminant.GetFactoryId(), icmm::CIlluminant::GetTypeId());
	QVERIFY(!icmm::CIlluminant::GetTypeId().isEmpty());
}


void CIlluminantTest::SerializeTest()
{
	// Standard illuminant round trip
	{
		icmm::CIlluminant source(icmm::StandardIlluminant::D65);

		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(source.Serialize(writeArchive));

		icmm::CIlluminant restored(icmm::StandardIlluminant::A);
		iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
		QVERIFY(restored.Serialize(readArchive));

		QVERIFY(restored.IsEqual(source));
		QCOMPARE(restored.GetIlluminantType(), icmm::StandardIlluminant::D65);
		QCOMPARE(restored.GetIlluminantName(), QString("D65"));
	}

	// Custom illuminant with white point round trip
	{
		icmm::CIlluminant source("Custom Illuminant", MakeWhitePoint(0.9505, 1.0, 1.089));

		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(source.Serialize(writeArchive));

		icmm::CIlluminant restored;
		iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
		QVERIFY(restored.Serialize(readArchive));

		QVERIFY(restored.IsEqual(source));
		QCOMPARE(restored.GetIlluminantType(), icmm::StandardIlluminant::Custom);
		QCOMPARE(restored.GetIlluminantName(), QString("Custom Illuminant"));
		QVERIFY(restored.GetWhitePoint() == source.GetWhitePoint());
	}
}


void CIlluminantTest::IsEqualTest()
{
	icmm::CIlluminant illuminant1(icmm::StandardIlluminant::D50);
	icmm::CIlluminant illuminant2(icmm::StandardIlluminant::D50);
	QVERIFY(illuminant1.IsEqual(illuminant2));
	QVERIFY(illuminant1.IsEqual(illuminant1));

	// Different type
	icmm::CIlluminant differentType(icmm::StandardIlluminant::D65);
	QVERIFY(!illuminant1.IsEqual(differentType));

	// Different name
	icmm::CIlluminant differentName(icmm::StandardIlluminant::D50);
	differentName.SetIlluminantName("Other");
	QVERIFY(!illuminant1.IsEqual(differentName));

	// Different white point
	icmm::CIlluminant differentWhitePoint(icmm::StandardIlluminant::D50);
	differentWhitePoint.SetWhitePoint(MakeWhitePoint(0.1, 0.2, 0.3));
	QVERIFY(!illuminant1.IsEqual(differentWhitePoint));

	// Comparison with an incompatible object type
	icmm::CVarColor otherObject(3);
	QVERIFY(!illuminant1.IsEqual(otherObject));
}


void CIlluminantTest::CopyFromTest()
{
	icmm::CIlluminant source("Source", MakeWhitePoint(0.4, 0.5, 0.6));

	icmm::CIlluminant target;
	QVERIFY(target.CopyFrom(source));
	QVERIFY(target.IsEqual(source));

	// Copying from an incompatible object fails
	icmm::CVarColor otherObject(3);
	QVERIFY(!target.CopyFrom(otherObject));
}


void CIlluminantTest::CloneMeTest()
{
	icmm::CIlluminant source("Source", MakeWhitePoint(0.4, 0.5, 0.6));

	istd::IChangeableUniquePtr clonePtr = source.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	QVERIFY(clonePtr->IsEqual(source));

	// The clone is an independent instance
	icmm::CIlluminant* clonedIlluminantPtr = dynamic_cast<icmm::CIlluminant*>(clonePtr.GetPtr());
	QVERIFY(clonedIlluminantPtr != nullptr);
	QVERIFY(clonedIlluminantPtr != &source);

	clonedIlluminantPtr->SetIlluminantName("Changed");
	QCOMPARE(source.GetIlluminantName(), QString("Source"));
}


I_ADD_TEST(CIlluminantTest);
