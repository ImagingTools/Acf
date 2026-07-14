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


namespace
{


class CLoggableCompactXmlFileReadArchive : public ifile::CCompactXmlFileReadArchive
{
public:
	mutable istd::IInformationProvider::InformationCategory messageCategory = istd::IInformationProvider::IC_NONE;
	mutable int messageId = 0;
	mutable QString message;

protected:
	virtual bool IsLogConsumed(
				const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
				const int* /*flagsPtr*/) const override
	{
		return true;
	}

	virtual bool SendLogMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& messageText,
				const QString& messageSource,
				int flags = 0) const override
	{
		QString decoratedMessage = messageText;
		QString decoratedMessageSource = messageSource;
		DecorateMessage(category, id, flags, decoratedMessage, decoratedMessageSource);

		messageCategory = category;
		messageId = id;
		message = decoratedMessage;

		return true;
	}
};


} // namespace


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


void CCompactXmlFileArchiveTest::DoOpenErrorDiagnosticTest()
{
	const QString testFilePath = "./MissingCompactXmlArchive/Archive.xml";
	CLoggableCompactXmlFileReadArchive readArchive;

	QVERIFY(!readArchive.OpenFile(testFilePath));
	QCOMPARE(readArchive.messageCategory, istd::IInformationProvider::IC_ERROR);
	QCOMPARE(readArchive.messageId, int(ifile::CCompactXmlFileReadArchive::MI_FILE_OPEN_ERROR));
	QVERIFY(readArchive.message.contains(testFilePath));
	QVERIFY(!readArchive.IsOpen());
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

