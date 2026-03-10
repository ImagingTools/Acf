// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CIChangeableTest.h"


// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <icmm/CIlluminant.h>
#include <icmm/CTristimulusSpecification.h>
#include <icmm/CSpectrumInfo.h>
#include <icmm/CRgbColorModel.h>
#include <icmm/CCieLabColorModel.h>
#include <icmm/CSpectralColorModel.h>
#include <icmm/CSpectralColorSpecificationBase.h>
#include <icmm/CSubstractiveColorModelBase.h>
#include <icmm/CCmyColorModel.h>
#include <icmm/CCmykColorModel.h>
#include <icmm/CVarColor.h>


// protected slots

void CIChangeableTest::initTestCase()
{
}


void CIChangeableTest::CIlluminantTest()
{
	// Test GetSupportedOperations
	icmm::CIlluminant illuminant1(icmm::StandardIlluminant::D65);
	int operations = illuminant1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom
	icmm::CIlluminant illuminant2;
	QVERIFY(illuminant2.CopyFrom(illuminant1));
	QVERIFY(illuminant2.IsEqual(illuminant1));
	QCOMPARE(illuminant2.GetIlluminantName(), illuminant1.GetIlluminantName());
	QCOMPARE(illuminant2.GetIlluminantType(), illuminant1.GetIlluminantType());

	// Test CloneMe
	istd::IChangeableUniquePtr clonePtr = illuminant1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	QVERIFY(clonePtr->IsEqual(illuminant1));

	// Test that cloned object is different instance
	icmm::CIlluminant* clonedIlluminant = dynamic_cast<icmm::CIlluminant*>(clonePtr.GetPtr());
	QVERIFY(clonedIlluminant != nullptr);
	QCOMPARE(clonedIlluminant->GetIlluminantType(), illuminant1.GetIlluminantType());
}


void CIChangeableTest::CTristimulusSpecificationTest()
{
	// Test GetSupportedOperations
	icmm::CTristimulusSpecification spec1(
		icmm::ObserverType::TwoDegree,
		icmm::AstmTableType::E308Table5,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D50)
	);
	int operations = spec1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom with CM_WITHOUT_REFS (default - doesn't copy external resources)
	icmm::CTristimulusSpecification spec2;
	QVERIFY(spec2.CopyFrom(spec1, istd::IChangeable::CM_WITHOUT_REFS));
	QCOMPARE(spec2.GetObserverType(), spec1.GetObserverType());
	QCOMPARE(spec2.GetMethod(), spec1.GetMethod());

	// Test CopyFrom with CM_WITH_REFS (copies external resources)
	icmm::CTristimulusSpecification spec3;
	QVERIFY(spec3.CopyFrom(spec1, istd::IChangeable::CM_WITH_REFS));
	QVERIFY(spec3.IsEqual(spec1));
	QCOMPARE(spec3.GetObserverType(), spec1.GetObserverType());
	QCOMPARE(spec3.GetMethod(), spec1.GetMethod());

	// Test CloneMe
	istd::IChangeableUniquePtr clonePtr = spec1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	// Note: CloneMe uses default mode (CM_WITHOUT_REFS), so won't be fully equal
}


void CIChangeableTest::CSpectrumInfoTest()
{
	// Test GetSupportedOperations
	icmm::CSpectrumInfo info1(istd::CIntRange(400, 700), 10);
	int operations = info1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);
	QVERIFY((operations & istd::IChangeable::SO_RESET) != 0);

	// Test CopyFrom
	icmm::CSpectrumInfo info2;
	QVERIFY(info2.CopyFrom(info1));
	QVERIFY(info2.IsEqual(info1));
	QCOMPARE(info2.GetSpectralRange(), info1.GetSpectralRange());
	QCOMPARE(info2.GetStep(), info1.GetStep());

	// Test CloneMe
	istd::IChangeableUniquePtr clonePtr = info1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	QVERIFY(clonePtr->IsEqual(info1));
}


void CIChangeableTest::CRgbColorModelTest()
{
	// Test GetSupportedOperations
	icmm::CRgbColorModel model1;
	int operations = model1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom
	icmm::CRgbColorModel model2;
	QVERIFY(model2.CopyFrom(model1));

	// Test CloneMe
	istd::IChangeableUniquePtr clonePtr = model1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	
	// Verify it's an RGB color model
	icmm::CRgbColorModel* clonedModel = dynamic_cast<icmm::CRgbColorModel*>(clonePtr.GetPtr());
	QVERIFY(clonedModel != nullptr);
	QCOMPARE(clonedModel->GetModelType(), icmm::IColorModel::MT_RGB);
}


void CIChangeableTest::CCieLabColorModelTest()
{
	// Test GetSupportedOperations
	icmm::CCieLabColorModel model1;
	int operations = model1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom
	icmm::CCieLabColorModel model2;
	QVERIFY(model2.CopyFrom(model1));

	// Test CloneMe
	istd::IChangeableUniquePtr clonePtr = model1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	
	// Verify it's a Lab color model
	icmm::CCieLabColorModel* clonedModel = dynamic_cast<icmm::CCieLabColorModel*>(clonePtr.GetPtr());
	QVERIFY(clonedModel != nullptr);
	QCOMPARE(clonedModel->GetModelType(), icmm::IColorModel::MT_LAB);
}


void CIChangeableTest::CSpectralColorModelTest()
{
	// Test GetSupportedOperations
	icmm::CSpectralColorModel model1;
	int operations = model1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom
	icmm::CSpectralColorModel model2;
	QVERIFY(model2.CopyFrom(model1));

	// Test CloneMe
	istd::IChangeableUniquePtr clonePtr = model1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	
	// Verify it's a spectral color model
	icmm::CSpectralColorModel* clonedModel = dynamic_cast<icmm::CSpectralColorModel*>(clonePtr.GetPtr());
	QVERIFY(clonedModel != nullptr);
	QCOMPARE(clonedModel->GetModelType(), icmm::IColorModel::MI_SPECTRAL);
}


void CIChangeableTest::CSpectralColorSpecificationBaseTest()
{
	// Create a derived class instance for testing
	class CTestSpectralColorSpec: public icmm::CSpectralColorSpecificationBase
	{
	public:
		CTestSpectralColorSpec(istd::CIntRange range, int step)
			: CSpectralColorSpecificationBase(range, step)
		{
		}
	};

	// Test GetSupportedOperations
	CTestSpectralColorSpec spec1(istd::CIntRange(400, 700), 10);
	int operations = spec1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom
	CTestSpectralColorSpec spec2(istd::CIntRange(300, 800), 5);
	QVERIFY(spec2.CopyFrom(spec1));
	QVERIFY(spec2.IsEqual(spec1));

	// Note: CloneMe is not tested here as base classes cannot implement it
	// Concrete derived classes should implement CloneMe
}


void CIChangeableTest::CSubstractiveColorModelBaseTest()
{
	// Create a derived class instance for testing
	// Using CCmyColorModel which is a concrete class derived from the base
	icmm::CCmyColorModel model1;
	int operations = model1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom
	icmm::CCmyColorModel model2;
	QVERIFY(model2.CopyFrom(model1));

	// Note: CloneMe is not tested here for the base class
	// It's tested in concrete derived classes like CCmyColorModel
}


void CIChangeableTest::CCmyColorModelTest()
{
	// Test GetSupportedOperations (inherited from base)
	icmm::CCmyColorModel model1;
	int operations = model1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom (inherited from base)
	icmm::CCmyColorModel model2;
	QVERIFY(model2.CopyFrom(model1));

	// Test CloneMe (override to return correct type)
	istd::IChangeableUniquePtr clonePtr = model1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	
	// Verify it's a CMY color model
	icmm::CCmyColorModel* clonedModel = dynamic_cast<icmm::CCmyColorModel*>(clonePtr.GetPtr());
	QVERIFY(clonedModel != nullptr);
	QCOMPARE(clonedModel->GetColorantIds().count(), 3); // C, M, Y
}


void CIChangeableTest::CCmykColorModelTest()
{
	// Test GetSupportedOperations (inherited from base)
	icmm::CCmykColorModel model1;
	int operations = model1.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);

	// Test CopyFrom (inherited from base)
	icmm::CCmykColorModel model2;
	QVERIFY(model2.CopyFrom(model1));

	// Test CloneMe (override to return correct type)
	istd::IChangeableUniquePtr clonePtr = model1.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	
	// Verify it's a CMYK color model
	icmm::CCmykColorModel* clonedModel = dynamic_cast<icmm::CCmykColorModel*>(clonePtr.GetPtr());
	QVERIFY(clonedModel != nullptr);
	QCOMPARE(clonedModel->GetColorantIds().count(), 4); // C, M, Y, K
}


void CIChangeableTest::cleanupTestCase()
{
}


I_ADD_TEST(CIChangeableTest);
