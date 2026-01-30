// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CJsonFileArchiveTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <ifile/CFileNameParam.h>
#include <imod/TModelWrap.h>
#include <ifile/CJsonFileReadArchive.h>
#include <ifile/CJsonFileWriteArchive.h>
#include <ifile/TFileSerializerComp.h>


void CJsonFileArchiveTest::DoBasicReadWriteTest()
{
	QString testFilePath = "./JsonFileOutput.json";
	QString path = "./Test/JsonFileTest.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CJsonFileWriteArchive writeArchive(testFilePath);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CJsonFileReadArchive readArchive(testFilePath);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);

	// Clean up
	QFile::remove(testFilePath);
}


void CJsonFileArchiveTest::DoFilePathTest()
{
	QString testFilePath = "./JsonFilePathTest.json";

	// Create a minimal valid JSON file using write archive
	QString path = "./Test/TempPath.test";
	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);
	
	{
		ifile::CJsonFileWriteArchive writeArchive(testFilePath);
		bool result = filePathParam.Serialize(writeArchive);
		QVERIFY(result);
	}

	// Verify the file was created and can be read
	QVERIFY(QFile::exists(testFilePath));
	
	{
		ifile::CJsonFileReadArchive readArchive(testFilePath);
		ifile::CFileNameParam filePathParam2;
		bool result = filePathParam2.Serialize(readArchive);
		QVERIFY(result);
	}

	// Clean up
	QFile::remove(testFilePath);
}


void CJsonFileArchiveTest::DoPersistenceComponentTest()
{
	typedef icomp::TSimComponentWrap<
				ifile::TFileSerializerComp<
							ifile::CJsonFileReadArchive,
							ifile::CJsonFileWriteArchive>> JsonFileSerializer;

	JsonFileSerializer component;
	component.InitComponent();

	QString testFilePath = "./JsonFilePersistenceOutput.json";
	QString path = "./Test/JsonFilePersistenceTest.test";
	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	{
		int state = component.SaveToFile(filePathParam, testFilePath);
		QVERIFY(state == ifile::IFilePersistence::OS_OK);
	}

	imod::TModelWrap<ifile::CFileNameParam> filePathParam2;
	int state = component.LoadFromFile(filePathParam2, testFilePath);
	QVERIFY(state == ifile::IFilePersistence::OS_OK);
	QVERIFY(filePathParam2.IsEqual(filePathParam));
	QVERIFY(filePathParam2.GetPath() == path);

	// Clean up
	QFile::remove(testFilePath);
}


I_ADD_TEST(CJsonFileArchiveTest);


