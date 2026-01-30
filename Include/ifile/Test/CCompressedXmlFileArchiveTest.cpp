// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CCompressedXmlFileArchiveTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <ifile/CFileNameParam.h>
#include <imod/TModelWrap.h>
#include <ifile/CCompressedXmlFileReadArchive.h>
#include <ifile/CCompressedXmlFileWriteArchive.h>
#include <ifile/TFileSerializerComp.h>


void CCompressedXmlFileArchiveTest::DoBasicReadWriteTest()
{
	QString testFilePath = "./CompressedXmlOutput.xml";
	QString path = "./Test/CompressedXmlTest.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CCompressedXmlFileWriteArchive writeArchive(testFilePath);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CCompressedXmlFileReadArchive readArchive(testFilePath);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);

	// Clean up
	QFile::remove(testFilePath);
}


void CCompressedXmlFileArchiveTest::DoFilePathTest()
{
	QString testFilePath = "./CompressedXmlFilePathTest.xml";

	// Write a compressed archive
	{
		ifile::CCompressedXmlFileWriteArchive writeArchive(testFilePath);
		QCOMPARE(writeArchive.GetCurrentFilePath(), testFilePath);
	}

	// Read the archive (requires a valid file to exist)
	// We need to create a minimal valid file first
	QString path = "./Test/TempPath.test";
	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);
	{
		ifile::CCompressedXmlFileWriteArchive writeArchive(testFilePath);
		filePathParam.Serialize(writeArchive);
	}

	ifile::CCompressedXmlFileReadArchive readArchive(testFilePath);
	QCOMPARE(readArchive.GetCurrentFilePath(), testFilePath);

	// Clean up
	QFile::remove(testFilePath);
}


void CCompressedXmlFileArchiveTest::DoPersistenceComponentTest()
{
	typedef icomp::TSimComponentWrap<
				ifile::TFileSerializerComp<
							ifile::CCompressedXmlFileReadArchive,
							ifile::CCompressedXmlFileWriteArchive>> CompressedXmlFileSerializer;

	CompressedXmlFileSerializer component;
	component.InitComponent();

	QString testFilePath = "./CompressedXmlPersistenceOutput.xml";
	QString path = "./Test/CompressedXmlPersistenceTest.test";
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


I_ADD_TEST(CCompressedXmlFileArchiveTest);


