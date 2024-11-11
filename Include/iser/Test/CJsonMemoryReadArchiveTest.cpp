#include <iser/Test/CJsonMemoryReadArchiveTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>


void CJsonMemoryReadArchiveTest::DoTest()
{
	Model model;
	model.value = 42;
	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(model.Serialize(writeArchive));
	QByteArray data = writeArchive.GetData();

	Model model2;
	model2.value = 0;

	iser::CJsonMemReadArchive readArchive(data);
	QVERIFY(model2.Serialize(readArchive));
	QVERIFY(model2.value == 42);
}

I_ADD_TEST(CJsonMemoryReadArchiveTest);
