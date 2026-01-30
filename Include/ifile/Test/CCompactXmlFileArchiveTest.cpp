// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CCompactXmlFileArchiveTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <ifile/CFileNameParam.h>
#include <imod/TModelWrap.h>
#include <ifile/CCompactXmlFileReadArchive.h>
#include <ifile/CCompactXmlFileWriteArchive.h>
#include <ifile/TFileSerializerComp.h>


void CCompactXmlFileArchiveTest::DoBasicReadWriteTest()
{
	QString testFilePath = "./CompactXmlFileOutput.xml";
	QString path = "./Test/CompactXmlFileTest.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CCompactXmlFileWriteArchive writeArchive(testFilePath);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CCompactXmlFileReadArchive readArchive(testFilePath);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);

	// Clean up
	QFile::remove(testFilePath);
}


void CCompactXmlFileArchiveTest::DoFilePathTest()
{
	QString testFilePath = "./CompactXmlFilePathTest.xml";

	// Write a compact archive
	{
		ifile::CCompactXmlFileWriteArchive writeArchive(testFilePath);
		QCOMPARE(writeArchive.GetCurrentFilePath(), testFilePath);
	}

	// Read the archive
	ifile::CCompactXmlFileReadArchive readArchive(testFilePath);
	QCOMPARE(readArchive.GetCurrentFilePath(), testFilePath);

	// Clean up
	QFile::remove(testFilePath);
}


void CCompactXmlFileArchiveTest::DoPersistenceComponentTest()
{
	typedef icomp::TSimComponentWrap<
				ifile::TFileSerializerComp<
							ifile::CCompactXmlFileReadArchive,
							ifile::CCompactXmlFileWriteArchive>> CompactXmlFileSerializer;

	CompactXmlFileSerializer component;
	component.InitComponent();

	QString testFilePath = "./CompactXmlFilePersistenceOutput.xml";
	QString path = "./Test/CompactXmlFilePersistenceTest.test";
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


I_ADD_TEST(CCompactXmlFileArchiveTest);


