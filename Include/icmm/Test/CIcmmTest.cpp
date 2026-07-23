// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CIcmmTest.h>


// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CCmy.h>
#include <icmm/CCmyk.h>
#include <icmm/CHsv.h>
#include <icmm/CLab.h>
#include <icmm/CRgb.h>
#include <icmm/icmm.h>


namespace
{

template<class T>
void TestSerialization(T source)
{
	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(source.Serialize(writeArchive));

	iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
	T restored;
	QVERIFY(restored.Serialize(readArchive));
	QVERIFY(restored == source);
}

}


void CIcmmTest::IsCmykTest()
{
	// Test CMYK colorants - should return true
	QVERIFY(icmm::IsCmyk(icmm::CU_CYAN));
	QVERIFY(icmm::IsCmyk(icmm::CU_DARK_CYAN));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_CYAN));
	QVERIFY(icmm::IsCmyk(icmm::CU_MAGENTA));
	QVERIFY(icmm::IsCmyk(icmm::CU_DARK_MAGENTA));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_MAGENTA));
	QVERIFY(icmm::IsCmyk(icmm::CU_YELLOW));
	QVERIFY(icmm::IsCmyk(icmm::CU_DARK_YELLOW));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_YELLOW));
	QVERIFY(icmm::IsCmyk(icmm::CU_BLACK));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_BLACK));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_LIGHT_BLACK));

	// Test non-CMYK colorants - should return false
	QVERIFY(!icmm::IsCmyk(icmm::CU_NONE));
	QVERIFY(!icmm::IsCmyk(icmm::CU_ECG));
	QVERIFY(!icmm::IsCmyk(icmm::CU_SPOT));
}


void CIcmmTest::CmySerializeTest()
{
	TestSerialization(icmm::CCmy(0.1, 0.2, 0.3));
}


void CIcmmTest::CmykSerializeTest()
{
	TestSerialization(icmm::CCmyk(0.1, 0.2, 0.3, 0.4));
}


void CIcmmTest::HsvSerializeTest()
{
	TestSerialization(icmm::CHsv(0.1, 0.2, 0.3));
}


void CIcmmTest::LabSerializeTest()
{
	TestSerialization(icmm::CLab(0.1, 0.2, 0.3));
}


void CIcmmTest::RgbSerializeTest()
{
	TestSerialization(icmm::CRgb(0.1, 0.2, 0.3));
}


I_ADD_TEST(CIcmmTest);

