// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CFileArchiveTest.h>


// Qt includes
#include <QtCore/QTemporaryFile>
#include <QtCore/QDir>

// ACF includes
#include <ifile/CFileReadArchive.h>
#include <ifile/CFileWriteArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class SimpleModel: virtual public iser::ISerializable
{
public:
	virtual bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag tag("Value", "Integer value");
		bool retVal = archive.BeginTag(tag);
		retVal = retVal && archive.Process(value);
		retVal = retVal && archive.EndTag(tag);
		return retVal;
	}

	int value = 0;
};


class ComplexModel: virtual public iser::ISerializable
{
public:
	virtual bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag intTag("IntValue", "Integer value");
		static iser::CArchiveTag doubleTag("DoubleValue", "Double value");
		static iser::CArchiveTag stringTag("StringValue", "String value");

		bool retVal = archive.BeginTag(intTag);
		retVal = retVal && archive.Process(intValue);
		retVal = retVal && archive.EndTag(intTag);

		retVal = retVal && archive.BeginTag(doubleTag);
		retVal = retVal && archive.Process(doubleValue);
		retVal = retVal && archive.EndTag(doubleTag);

		retVal = retVal && archive.BeginTag(stringTag);
		retVal = retVal && archive.Process(stringValue);
		retVal = retVal && archive.EndTag(stringTag);

		return retVal;
	}

	int intValue = 0;
	double doubleValue = 0.0;
	QString stringValue;

	bool operator==(const ComplexModel& other) const
	{
		return intValue == other.intValue && 
		       qFuzzyCompare(doubleValue, other.doubleValue) && 
		       stringValue == other.stringValue;
	}
};


void CFileArchiveTest::BasicSerializationTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data
	{
		SimpleModel writeModel;
		writeModel.value = 42;

		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data
	{
		SimpleModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QCOMPARE(readModel.value, 42);
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileArchiveTest::PrimitiveTypesTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data
	{
		ComplexModel writeModel;
		writeModel.intValue = 123;
		writeModel.doubleValue = 456.789;
		writeModel.stringValue = "Test String";

		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data
	{
		ComplexModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QVERIFY(readModel.intValue == 123);
		QVERIFY(qFuzzyCompare(readModel.doubleValue, 456.789));
		QCOMPARE(readModel.stringValue, QString("Test String"));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileArchiveTest::StringSerializationTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data
	{
		ComplexModel writeModel;
		writeModel.intValue = 999;
		writeModel.doubleValue = 3.14159;
		writeModel.stringValue = "Unicode test: äöü ñ 中文";

		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data
	{
		ComplexModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QCOMPARE(readModel.stringValue, QString("Unicode test: äöü ñ 中文"));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileArchiveTest::TagSkippingTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data with tag skipping
	{
		ComplexModel writeModel;
		writeModel.intValue = 100;
		writeModel.doubleValue = 200.5;
		writeModel.stringValue = "Tag Skip Test";

		ifile::CFileWriteArchive writeArchive(filePath, NULL, true);
		QVERIFY(writeArchive.IsTagSkippingSupported());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data with tag skipping
	{
		ComplexModel readModel;
		ifile::CFileReadArchive readArchive(filePath, true);
		QVERIFY(readArchive.IsTagSkippingSupported());
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QCOMPARE(readModel.intValue, 100);
		QVERIFY(qFuzzyCompare(readModel.doubleValue, 200.5));
		QCOMPARE(readModel.stringValue, QString("Tag Skip Test"));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileArchiveTest::InvalidFilePathTest()
{
	// Test writing to invalid path
	QString invalidPath = "/invalid/path/that/does/not/exist/test.dat";
	ifile::CFileWriteArchive writeArchive(invalidPath);
	QVERIFY(!writeArchive.IsArchiveValid());
}


void CFileArchiveTest::MultipleObjectsTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write multiple objects
	{
		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		
		SimpleModel model1;
		model1.value = 10;
		QVERIFY(model1.Serialize(writeArchive));
		
		SimpleModel model2;
		model2.value = 20;
		QVERIFY(model2.Serialize(writeArchive));
		
		SimpleModel model3;
		model3.value = 30;
		QVERIFY(model3.Serialize(writeArchive));
	}
	
	// Read multiple objects
	{
		ifile::CFileReadArchive readArchive(filePath);
		
		SimpleModel model1;
		QVERIFY(model1.Serialize(readArchive));
		QCOMPARE(model1.value, 10);
		
		SimpleModel model2;
		QVERIFY(model2.Serialize(readArchive));
		QCOMPARE(model2.value, 20);
		
		SimpleModel model3;
		QVERIFY(model3.Serialize(readArchive));
		QCOMPARE(model3.value, 30);
	}
	
	// Clean up
	QFile::remove(filePath);
}


I_ADD_TEST(CFileArchiveTest);


