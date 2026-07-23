// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CColorModelTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <icmm/CCieLabColorModel.h>
#include <icmm/CCmyColorModel.h>
#include <icmm/CCmykColorModel.h>
#include <icmm/CRgbColorModel.h>
#include <icmm/CRgbaColorModel.h>
#include <icmm/CSpectralColorModel.h>
#include <icmm/CSpectralColorSpecificationBase.h>


namespace
{

// Helper exposing the protected range/step constructor for testing
class CTestSpectralColorSpecification: public icmm::CSpectralColorSpecificationBase
{
public:
	CTestSpectralColorSpecification(const istd::CIntRange& range, int step)
		:CSpectralColorSpecificationBase(range, step)
	{
	}
};

}


void CColorModelTest::RgbColorModelTest()
{
	icmm::CRgbColorModel model;

	QCOMPARE(model.GetModelType(), icmm::IColorModel::MT_RGB);
	QCOMPARE(model.GetModelClass(), icmm::IColorModel::MC_DEVICE_DEPENDENT);
	QCOMPARE(model.GetColorSpaceClass(), icmm::IColorModel::CSC_ADDITIVE);
	QCOMPARE(model.GetColorSpaceDimensionality(), 3);

	QCOMPARE(model.GetColorSpaceComponentName(0), QString("R"));
	QCOMPARE(model.GetColorSpaceComponentName(1), QString("G"));
	QCOMPARE(model.GetColorSpaceComponentName(2), QString("B"));

	QVERIFY(model.GetColorSpaceComponentInfo(0) != nullptr);
	QVERIFY(model.GetSpecification() != nullptr);

	// IChangeable operations
	istd::IChangeableUniquePtr clonePtr = model.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);

	icmm::CRgbColorModel copy;
	QVERIFY(copy.CopyFrom(model));
}


void CColorModelTest::RgbaColorModelTest()
{
	icmm::CRgbaColorModel model;

	// RGBA extends RGB by the alpha channel
	QCOMPARE(model.GetModelType(), icmm::IColorModel::MT_RGB);
	QCOMPARE(model.GetColorSpaceDimensionality(), 4);
	QCOMPARE(model.GetColorSpaceComponentName(0), QString("R"));
	QCOMPARE(model.GetColorSpaceComponentName(3), QString("A"));
}


void CColorModelTest::CieLabColorModelTest()
{
	icmm::CCieLabColorModel model;

	QCOMPARE(model.GetModelType(), icmm::IColorModel::MT_LAB);
	QCOMPARE(model.GetModelClass(), icmm::IColorModel::MC_DEVICE_INDEPENDENT);
	QCOMPARE(model.GetColorSpaceClass(), icmm::IColorModel::CSC_PERCEPTUAL);
	QCOMPARE(model.GetColorSpaceDimensionality(), 3);

	QCOMPARE(model.GetColorSpaceComponentName(0), QString("L"));
	QCOMPARE(model.GetColorSpaceComponentName(1), QString("a"));
	QCOMPARE(model.GetColorSpaceComponentName(2), QString("b"));

	QVERIFY(model.GetColorSpaceComponentInfo(0) != nullptr);
}


void CColorModelTest::CmyColorModelTest()
{
	icmm::CCmyColorModel model;

	QCOMPARE(model.GetModelType(), icmm::IColorModel::MT_COLORANTS);
	QCOMPARE(model.GetModelClass(), icmm::IColorModel::MC_DEVICE_DEPENDENT);
	QCOMPARE(model.GetColorSpaceClass(), icmm::IColorModel::CSC_SUBSTRACTIVE);
	QCOMPARE(model.GetColorSpaceDimensionality(), 3);

	icmm::IColorantList::ColorantIds colorantIds = model.GetColorantIds();
	QCOMPARE(colorantIds.count(), 3);
	QVERIFY(colorantIds.contains(icmm::CCmyColorModel::GetCyan()));
	QVERIFY(colorantIds.contains(icmm::CCmyColorModel::GetMagenta()));
	QVERIFY(colorantIds.contains(icmm::CCmyColorModel::GetYellow()));
}


void CColorModelTest::CmykColorModelTest()
{
	icmm::CCmykColorModel model;

	QCOMPARE(model.GetModelType(), icmm::IColorModel::MT_COLORANTS);
	QCOMPARE(model.GetColorSpaceDimensionality(), 4);

	icmm::IColorantList::ColorantIds colorantIds = model.GetColorantIds();
	QCOMPARE(colorantIds.count(), 4);
	QVERIFY(colorantIds.contains(icmm::CCmyColorModel::GetCyan()));
	QVERIFY(colorantIds.contains(icmm::CCmyColorModel::GetMagenta()));
	QVERIFY(colorantIds.contains(icmm::CCmyColorModel::GetYellow()));
	QVERIFY(colorantIds.contains(icmm::CCmykColorModel::GetBlack()));
}


void CColorModelTest::SpectralColorModelTest()
{
	// Model without specification has no dimensions
	icmm::CSpectralColorModel emptyModel;
	QCOMPARE(emptyModel.GetModelType(), icmm::IColorModel::MI_SPECTRAL);
	QCOMPARE(emptyModel.GetModelClass(), icmm::IColorModel::MC_DEVICE_INDEPENDENT);
	QCOMPARE(emptyModel.GetColorSpaceClass(), icmm::IColorModel::CSC_PERCEPTUAL);
	QCOMPARE(emptyModel.GetColorSpaceDimensionality(), 0);
	QVERIFY(emptyModel.GetSpecification() == nullptr);

	// Model with a spectral specification
	auto spec = std::make_shared<CTestSpectralColorSpecification>(istd::CIntRange(400, 700), 10);
	icmm::CSpectralColorModel model(spec);
	QCOMPARE(model.GetColorSpaceDimensionality(), 31);
	QVERIFY(model.GetSpecification() == spec);
}


void CColorModelTest::CreateColorTransformationTest()
{
	icmm::CRgbColorModel rgbModel;

	// RGB -> CMYK transformation is available
	icmm::CCmykColorModel cmykModel;
	std::unique_ptr<const icmm::IColorTransformation> rgbToCmykPtr(
		rgbModel.CreateColorTranformation(cmykModel, QByteArray()));
	QVERIFY(rgbToCmykPtr != nullptr);

	// The created transformation converts white to zero ink coverage
	icmm::CVarColor white(3, 1.0);
	icmm::CVarColor cmyk(4);
	QVERIFY(rgbToCmykPtr->GetValueAt(white, cmyk));
	for (int i = 0; i < 4; ++i){
		QVERIFY(qAbs(cmyk.GetElement(i)) < 0.001);
	}

	// No transformation to unsupported models
	icmm::CCieLabColorModel labModel;
	QVERIFY(rgbModel.CreateColorTranformation(labModel, QByteArray()) == nullptr);

	// CieLab model does not provide any transformations
	QVERIFY(labModel.CreateColorTranformation(rgbModel, QByteArray()) == nullptr);
}


I_ADD_TEST(CColorModelTest);
