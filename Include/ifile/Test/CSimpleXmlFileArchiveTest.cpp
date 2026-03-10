// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CSimpleXmlFileArchiveTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <ifile/CFileNameParam.h>
#include <imod/TModelWrap.h>
#include <ifile/CSimpleXmlFileReadArchive.h>
#include <ifile/CSimpleXmlFileWriteArchive.h>
#include <ifile/TFileSerializerComp.h>


void CSimpleXmlFileArchiveTest::DoBasicReadWriteTest()
{
	QString testFilePath = "./SimpleXmlOutput.xml";
	QString path = "./Test/SimpleXmlTest.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CSimpleXmlFileWriteArchive writeArchive(testFilePath);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CSimpleXmlFileReadArchive readArchive(testFilePath);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);

	// Clean up
	QFile::remove(testFilePath);
}


void CSimpleXmlFileArchiveTest::DoFilePathTest()
{
	QString testFilePath = "./SimpleXmlFilePathTest.xml";

	// Write a simple archive
	{
		ifile::CSimpleXmlFileWriteArchive writeArchive(testFilePath);
		QCOMPARE(writeArchive.GetCurrentFilePath(), testFilePath);
	}

	// Read the archive
	ifile::CSimpleXmlFileReadArchive readArchive(testFilePath);
	QCOMPARE(readArchive.GetCurrentFilePath(), testFilePath);

	// Clean up
	QFile::remove(testFilePath);
}


void CSimpleXmlFileArchiveTest::DoPersistenceComponentTest()
{
	typedef icomp::TSimComponentWrap<
				ifile::TFileSerializerComp<
							ifile::CSimpleXmlFileReadArchive,
							ifile::CSimpleXmlFileWriteArchive>> SimpleXmlFileSerializer;

	SimpleXmlFileSerializer component;
	component.InitComponent();

	QString testFilePath = "./SimpleXmlPersistenceOutput.xml";
	QString path = "./Test/SimpleXmlPersistenceTest.test";
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


I_ADD_TEST(CSimpleXmlFileArchiveTest);


