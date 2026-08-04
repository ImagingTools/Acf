// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CSpectrumTest.h>


// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CSpectrum.h>
#include <icmm/CSpectrumInfo.h>


// CSpectrumInfo

void CSpectrumTest::SpectrumInfoConstructorTest()
{
	// Default constructor creates an empty spectrum info
	icmm::CSpectrumInfo defaultInfo;
	QCOMPARE(defaultInfo.GetStep(), 0);
	QCOMPARE(defaultInfo.GetSamplesCount(), 0);

	// Constructor with range and step
	icmm::CSpectrumInfo info(istd::CIntRange(380, 780), 10);
	QCOMPARE(info.GetSpectralRange().GetMinValue(), 380);
	QCOMPARE(info.GetSpectralRange().GetMaxValue(), 780);
	QCOMPARE(info.GetStep(), 10);
	QCOMPARE(info.GetSamplesCount(), 41);

	// Copy constructor via interface
	icmm::CSpectrumInfo copy(static_cast<const icmm::ISpectrumInfo&>(info));
	QVERIFY(copy.IsEqual(info));
}


void CSpectrumTest::SpectrumInfoSetRangeTest()
{
	icmm::CSpectrumInfo info(istd::CIntRange(400, 700), 10);

	// A range compatible with the current step is accepted
	QVERIFY(info.SetRange(istd::CIntRange(380, 780)));
	QCOMPARE(info.GetSpectralRange().GetMinValue(), 380);
	QCOMPARE(info.GetSpectralRange().GetMaxValue(), 780);

	// A range incompatible with the current step is rejected
	QVERIFY(!info.SetRange(istd::CIntRange(400, 705)));
	QCOMPARE(info.GetSpectralRange().GetMinValue(), 380);
	QCOMPARE(info.GetSpectralRange().GetMaxValue(), 780);
}


void CSpectrumTest::SpectrumInfoSetStepTest()
{
	icmm::CSpectrumInfo info(istd::CIntRange(400, 700), 10);

	// A step dividing the range length is accepted
	QVERIFY(info.SetStep(5));
	QCOMPARE(info.GetStep(), 5);

	QVERIFY(info.SetStep(300));
	QCOMPARE(info.GetStep(), 300);

	// A step not dividing the range length is rejected
	QVERIFY(!info.SetStep(7));
	QCOMPARE(info.GetStep(), 300);
}


void CSpectrumTest::SpectrumInfoSamplesCountTest()
{
	// Samples count includes both range boundaries
	QCOMPARE(icmm::CSpectrumInfo(istd::CIntRange(400, 700), 100).GetSamplesCount(), 4);
	QCOMPARE(icmm::CSpectrumInfo(istd::CIntRange(400, 700), 300).GetSamplesCount(), 2);
	QCOMPARE(icmm::CSpectrumInfo(istd::CIntRange(380, 780), 10).GetSamplesCount(), 41);

	// Uninitialized info (step 0) has no samples
	QCOMPARE(icmm::CSpectrumInfo().GetSamplesCount(), 0);
}


void CSpectrumTest::SpectrumInfoSerializeTest()
{
	icmm::CSpectrumInfo source(istd::CIntRange(380, 780), 20);

	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(source.Serialize(writeArchive));

	icmm::CSpectrumInfo restored;
	iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
	QVERIFY(restored.Serialize(readArchive));

	QVERIFY(restored.IsEqual(source));
	QCOMPARE(restored.GetStep(), 20);
	QCOMPARE(restored.GetSamplesCount(), 21);
}


void CSpectrumTest::SpectrumInfoChangeableTest()
{
	icmm::CSpectrumInfo source(istd::CIntRange(400, 700), 10);

	int operations = source.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);
	QVERIFY((operations & istd::IChangeable::SO_RESET) != 0);

	// CopyFrom
	icmm::CSpectrumInfo copy;
	QVERIFY(copy.CopyFrom(source));
	QVERIFY(copy.IsEqual(source));

	// IsEqual negative cases
	icmm::CSpectrumInfo different(istd::CIntRange(400, 700), 30);
	QVERIFY(!source.IsEqual(different));

	// CloneMe
	istd::IChangeableUniquePtr clonePtr = source.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	QVERIFY(clonePtr->IsEqual(source));

	// ResetData
	QVERIFY(copy.ResetData());
	QCOMPARE(copy.GetStep(), 0);
	QCOMPARE(copy.GetSamplesCount(), 0);
	QVERIFY(!copy.IsEqual(source));
}


// CSpectrum

void CSpectrumTest::SpectrumConstructorTest()
{
	// Construction from explicit samples
	std::vector<double> samples = {0.1, 0.2, 0.3, 0.4};
	icmm::CSpectrum spectrum(400, 700, samples);

	QCOMPARE(spectrum.GetSamplesCount(), 4);
	QCOMPARE(spectrum.GetStep(), 100);
	QCOMPARE(spectrum.GetSpectralRange().GetMinValue(), 400);
	QCOMPARE(spectrum.GetSpectralRange().GetMaxValue(), 700);
	QCOMPARE(spectrum.GetSampleValue(0), 0.1);
	QCOMPARE(spectrum.GetSampleValue(3), 0.4);

	// Construction with a step creates zero-initialized samples
	icmm::CSpectrum zeroSpectrum(380, 780, 10);
	QCOMPARE(zeroSpectrum.GetSamplesCount(), 41);
	QCOMPARE(zeroSpectrum.GetStep(), 10);
	for (int i = 0; i < zeroSpectrum.GetSamplesCount(); ++i){
		QCOMPARE(zeroSpectrum.GetSampleValue(i), 0.0);
	}

	// Copy constructor
	icmm::CSpectrum copy(spectrum);
	QVERIFY(copy.IsEqual(spectrum));

	// The spectrum info refers to the spectrum itself
	QVERIFY(spectrum.GetSpectrumInfo() != nullptr);
	QCOMPARE(spectrum.GetSpectrumInfo()->GetSamplesCount(), 4);
}


void CSpectrumTest::SpectrumInvalidConstructionTest()
{
	// A step that does not divide the wavelength range leaves the spectrum empty
	icmm::CSpectrum invalidStep(400, 700, 7);
	QCOMPARE(invalidStep.GetSamplesCount(), 0);
}


void CSpectrumTest::SpectrumSampleAccessTest()
{
	icmm::CSpectrum spectrum(400, 600, 100);
	QCOMPARE(spectrum.GetSamplesCount(), 3);

	spectrum.SetSampleValue(0, 0.0);
	spectrum.SetSampleValue(1, 0.5);
	spectrum.SetSampleValue(2, 1.0);

	QCOMPARE(spectrum.GetSampleValue(1), 0.5);

	std::vector<double> samples = spectrum.GetSamples();
	QCOMPARE(int(samples.size()), 3);
	QCOMPARE(samples[2], 1.0);

	// Sample coordinates follow the wavelength range
	QCOMPARE(spectrum.GetSampleCoordinate(0), 400.0);
	QCOMPARE(spectrum.GetSampleCoordinate(2), 600.0);
}


void CSpectrumTest::SpectrumSerializeTest()
{
	std::vector<double> samples = {0.0, 0.25, 0.5, 0.75, 1.0};
	icmm::CSpectrum source(400, 800, samples);

	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(source.Serialize(writeArchive));

	icmm::CSpectrum restored;
	iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
	QVERIFY(restored.Serialize(readArchive));

	QCOMPARE(restored.GetSamplesCount(), 5);
	QCOMPARE(restored.GetSampleValue(1), 0.25);
	QCOMPARE(restored.GetSpectralRange().GetMinValue(), 400);
	QCOMPARE(restored.GetSpectralRange().GetMaxValue(), 800);
	QVERIFY(restored.IsEqual(source));
}


void CSpectrumTest::SpectrumChangeableTest()
{
	std::vector<double> samples = {0.1, 0.2, 0.3};
	icmm::CSpectrum source(400, 600, samples);

	int operations = source.GetSupportedOperations();
	QVERIFY((operations & istd::IChangeable::SO_CLONE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COMPARE) != 0);
	QVERIFY((operations & istd::IChangeable::SO_COPY) != 0);
	QVERIFY((operations & istd::IChangeable::SO_RESET) != 0);

	// CopyFrom
	icmm::CSpectrum copy;
	QVERIFY(copy.CopyFrom(source));
	QVERIFY(copy.IsEqual(source));

	// IsEqual negative case: different sample values
	icmm::CSpectrum different(400, 600, std::vector<double>{0.1, 0.2, 0.4});
	QVERIFY(!source.IsEqual(different));

	// CopyFrom incompatible object fails
	icmm::CSpectrumInfo info(istd::CIntRange(400, 600), 100);
	QVERIFY(!copy.CopyFrom(info));

	// CloneMe
	istd::IChangeableUniquePtr clonePtr = source.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	QVERIFY(clonePtr->IsEqual(source));

	// ResetData
	QVERIFY(copy.ResetData());
	QVERIFY(!copy.IsEqual(source));
}


I_ADD_TEST(CSpectrumTest);
