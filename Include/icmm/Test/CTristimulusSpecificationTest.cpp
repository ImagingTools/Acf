// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CTristimulusSpecificationTest.h>


// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CIlluminant.h>
#include <icmm/CTristimulusSpecification.h>


void CTristimulusSpecificationTest::DefaultConstructorTest()
{
	icmm::CTristimulusSpecification spec;

	QCOMPARE(spec.GetObserverType(), icmm::ObserverType::TwoDegree);
	QCOMPARE(spec.GetMethod(), icmm::AstmTableType::Unknown);

	// A default D50 illuminant is created when none is provided
	std::shared_ptr<icmm::IIlluminant> illuminantPtr = spec.GetIlluminant();
	QVERIFY(illuminantPtr != nullptr);
	QCOMPARE(illuminantPtr->GetIlluminantType(), icmm::StandardIlluminant::D50);

	// No base specification by default
	QVERIFY(spec.GetBaseSpecification() == nullptr);
}


void CTristimulusSpecificationTest::ParameterConstructorTest()
{
	std::shared_ptr<icmm::IIlluminant> illuminantPtr =
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D65);

	icmm::CTristimulusSpecification spec(
		icmm::ObserverType::TenDegree,
		illuminantPtr,
		icmm::AstmTableType::E308Table5);

	QCOMPARE(spec.GetObserverType(), icmm::ObserverType::TenDegree);
	QCOMPARE(spec.GetMethod(), icmm::AstmTableType::E308Table5);
	QVERIFY(spec.GetIlluminant() == illuminantPtr);
	QVERIFY(spec.GetBaseSpecification() == nullptr);
}


void CTristimulusSpecificationTest::CopyConstructorTest()
{
	std::shared_ptr<icmm::IIlluminant> illuminantPtr =
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D65);

	icmm::CTristimulusSpecification source(
		icmm::ObserverType::TenDegree,
		illuminantPtr,
		icmm::AstmTableType::E3081nm);

	icmm::CTristimulusSpecification copy(source);
	QCOMPARE(copy.GetObserverType(), source.GetObserverType());
	QCOMPARE(copy.GetMethod(), source.GetMethod());
	QVERIFY(copy.GetIlluminant() == source.GetIlluminant());
	QVERIFY(copy == source);
}


void CTristimulusSpecificationTest::FactoryIdTest()
{
	icmm::CTristimulusSpecification spec;
	QCOMPARE(spec.GetFactoryId(), icmm::CTristimulusSpecification::GetTypeId());
	QVERIFY(!icmm::CTristimulusSpecification::GetTypeId().isEmpty());
}


void CTristimulusSpecificationTest::EqualityTest()
{
	icmm::CTristimulusSpecification spec1;
	icmm::CTristimulusSpecification spec2;

	// Independent default specifications have equal (D50) illuminants
	QVERIFY(spec1 == spec2);
	QVERIFY(spec1.IsEqual(spec2));

	// Different observer type
	icmm::CTristimulusSpecification differentObserver(icmm::ObserverType::TenDegree);
	QVERIFY(!(spec1 == differentObserver));
	QVERIFY(!spec1.IsEqual(differentObserver));

	// Different method
	icmm::CTristimulusSpecification differentMethod(
		icmm::ObserverType::TwoDegree, nullptr, icmm::AstmTableType::E308Table6);
	QVERIFY(!(spec1 == differentMethod));

	// Different illuminant
	icmm::CTristimulusSpecification differentIlluminant(
		icmm::ObserverType::TwoDegree,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D65));
	QVERIFY(!(spec1 == differentIlluminant));

	// Comparison with an incompatible object type
	icmm::CIlluminant illuminant;
	QVERIFY(!spec1.IsEqual(illuminant));
}


void CTristimulusSpecificationTest::CopyFromTest()
{
	icmm::CTristimulusSpecification source(
		icmm::ObserverType::TenDegree,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D65),
		icmm::AstmTableType::E308Table5);

	int operations = source.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	icmm::CTristimulusSpecification target;
	QVERIFY(target.CopyFrom(source));
	QCOMPARE(target.GetObserverType(), icmm::ObserverType::TenDegree);
	QCOMPARE(target.GetMethod(), icmm::AstmTableType::E308Table5);
	QVERIFY(target.GetIlluminant() == source.GetIlluminant());

	// Copying from an incompatible object fails
	icmm::CIlluminant illuminant;
	QVERIFY(!target.CopyFrom(illuminant));
}


void CTristimulusSpecificationTest::CloneMeTest()
{
	icmm::CTristimulusSpecification source(
		icmm::ObserverType::TenDegree,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D65),
		icmm::AstmTableType::E308Table6);

	istd::IChangeableUniquePtr clonePtr = source.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);

	icmm::CTristimulusSpecification* clonedSpecPtr =
		dynamic_cast<icmm::CTristimulusSpecification*>(clonePtr.GetPtr());
	QVERIFY(clonedSpecPtr != nullptr);
	QVERIFY(clonedSpecPtr != &source);
	QCOMPARE(clonedSpecPtr->GetObserverType(), source.GetObserverType());
	QCOMPARE(clonedSpecPtr->GetMethod(), source.GetMethod());
	QVERIFY(clonedSpecPtr->GetIlluminant() == source.GetIlluminant());
}


void CTristimulusSpecificationTest::SerializeTest()
{
	icmm::CTristimulusSpecification source(
		icmm::ObserverType::TenDegree,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D65),
		icmm::AstmTableType::E308Table5);

	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(source.Serialize(writeArchive));

	icmm::CTristimulusSpecification restored;
	iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
	QVERIFY(restored.Serialize(readArchive));

	QCOMPARE(restored.GetObserverType(), icmm::ObserverType::TenDegree);
	QCOMPARE(restored.GetMethod(), icmm::AstmTableType::E308Table5);
	QVERIFY(restored.GetIlluminant() != nullptr);
	QCOMPARE(restored.GetIlluminant()->GetIlluminantType(), icmm::StandardIlluminant::D65);
}


void CTristimulusSpecificationTest::GetD50TwoDegreeTest()
{
	const icmm::CTristimulusSpecification& spec = icmm::CTristimulusSpecification::GetD50TwoDegree();

	QCOMPARE(spec.GetObserverType(), icmm::ObserverType::TwoDegree);
	QCOMPARE(spec.GetMethod(), icmm::AstmTableType::Unknown);
	QVERIFY(spec.GetIlluminant() != nullptr);
	QCOMPARE(spec.GetIlluminant()->GetIlluminantType(), icmm::StandardIlluminant::D50);

	// The same instance is returned on subsequent calls
	QVERIFY(&spec == &icmm::CTristimulusSpecification::GetD50TwoDegree());
}


I_ADD_TEST(CTristimulusSpecificationTest);
