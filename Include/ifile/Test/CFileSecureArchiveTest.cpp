// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CFileSecureArchiveTest.h>


// Qt includes
#include <QtCore/QTemporaryFile>
#include <QtCore/QFile>

// ACF includes
#include <ifile/CFileReadSecureArchive.h>
#include <ifile/CFileWriteSecureArchive.h>
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


void CFileSecureArchiveTest::BasicSecureSerializationTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data with secure archive
	{
		SimpleModel writeModel;
		writeModel.value = 42;

		ifile::CFileWriteSecureArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data with secure archive
	{
		SimpleModel readModel;
		ifile::CFileReadSecureArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QCOMPARE(readModel.value, 42);
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileSecureArchiveTest::PrimitiveTypesSecureTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data with secure archive
	{
		ComplexModel writeModel;
		writeModel.intValue = 123;
		writeModel.doubleValue = 456.789;
		writeModel.stringValue = "Secure Test String";

		ifile::CFileWriteSecureArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data with secure archive
	{
		ComplexModel readModel;
		ifile::CFileReadSecureArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QVERIFY(readModel.intValue == 123);
		QVERIFY(qFuzzyCompare(readModel.doubleValue, 456.789));
		QCOMPARE(readModel.stringValue, QString("Secure Test String"));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileSecureArchiveTest::StringSecureSerializationTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data with secure archive
	{
		ComplexModel writeModel;
		writeModel.intValue = 999;
		writeModel.doubleValue = 3.14159;
		writeModel.stringValue = "Secure Unicode: äöü ñ 中文 🔒";

		ifile::CFileWriteSecureArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data with secure archive
	{
		ComplexModel readModel;
		ifile::CFileReadSecureArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));

		// Verify
		QCOMPARE(readModel.stringValue, QString("Secure Unicode: äöü ñ 中文 🔒"));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileSecureArchiveTest::SecureArchiveIntegrityTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data with secure archive
	{
		SimpleModel writeModel;
		writeModel.value = 12345;

		ifile::CFileWriteSecureArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Try to read with normal archive (should fail or give wrong data)
	// This tests that the encoding is actually applied
	{
		SimpleModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		// Don't verify result - encoded data won't deserialize correctly with normal archive
		readModel.Serialize(readArchive);
		
		// The value should NOT match because the file is encoded
		QVERIFY(readModel.value != 12345);
	}
	
	// Now read with secure archive - should work
	{
		SimpleModel readModel;
		ifile::CFileReadSecureArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));
		QCOMPARE(readModel.value, 12345);
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CFileSecureArchiveTest::CompareSecureVsNormalTest()
{
	// Create temporary files
	QTemporaryFile normalFile;
	QVERIFY(normalFile.open());
	QString normalPath = normalFile.fileName();
	normalFile.close();
	
	QTemporaryFile secureFile;
	QVERIFY(secureFile.open());
	QString securePath = secureFile.fileName();
	secureFile.close();
	
	// Write same data to both archives
	SimpleModel writeModel;
	writeModel.value = 777;
	
	{
		ifile::CFileWriteArchive normalArchive(normalPath);
		QVERIFY(normalArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(normalArchive));
	}
	
	{
		ifile::CFileWriteSecureArchive secureArchive(securePath);
		QVERIFY(secureArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(secureArchive));
	}
	
	// Read both files raw and verify they're different
	QFile normalFileObj(normalPath);
	QFile secureFileObj(securePath);
	
	QVERIFY(normalFileObj.open(QIODevice::ReadOnly));
	QVERIFY(secureFileObj.open(QIODevice::ReadOnly));
	
	QByteArray normalData = normalFileObj.readAll();
	QByteArray secureData = secureFileObj.readAll();
	
	normalFileObj.close();
	secureFileObj.close();
	
	// Files should have same size but different content (due to encoding)
	QCOMPARE(normalData.size(), secureData.size());
	QVERIFY(normalData != secureData);
	
	// Clean up
	QFile::remove(normalPath);
	QFile::remove(securePath);
}


void CFileSecureArchiveTest::MultipleObjectsSecureTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write multiple objects with secure archive
	{
		ifile::CFileWriteSecureArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		
		SimpleModel model1;
		model1.value = 100;
		QVERIFY(model1.Serialize(writeArchive));
		
		SimpleModel model2;
		model2.value = 200;
		QVERIFY(model2.Serialize(writeArchive));
		
		SimpleModel model3;
		model3.value = 300;
		QVERIFY(model3.Serialize(writeArchive));
	}
	
	// Read multiple objects with secure archive
	{
		ifile::CFileReadSecureArchive readArchive(filePath);
		
		SimpleModel model1;
		QVERIFY(model1.Serialize(readArchive));
		QCOMPARE(model1.value, 100);
		
		SimpleModel model2;
		QVERIFY(model2.Serialize(readArchive));
		QCOMPARE(model2.value, 200);
		
		SimpleModel model3;
		QVERIFY(model3.Serialize(readArchive));
		QCOMPARE(model3.value, 300);
	}
	
	// Clean up
	QFile::remove(filePath);
}


I_ADD_TEST(CFileSecureArchiveTest);


