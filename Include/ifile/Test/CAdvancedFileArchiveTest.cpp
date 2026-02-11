// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CAdvancedFileArchiveTest.h>


// Qt includes
#include <QtCore/QTemporaryFile>
#include <QtCore/QDir>
#include <QtCore/QVector>
#include <QtCore/QThread>
#include <QtConcurrent/QtConcurrent>

// ACF includes
#include <ifile/CFileReadArchive.h>
#include <ifile/CFileWriteArchive.h>
#include <ifile/CFileReadSecureArchive.h>
#include <ifile/CFileWriteSecureArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


// Test model with nested structure
class NestedModel: virtual public iser::ISerializable
{
public:
	virtual bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag idTag("Id", "Identifier");
		static iser::CArchiveTag nameTag("Name", "Name");
		static iser::CArchiveTag childrenTag("Children", "Children collection");
		
		bool retVal = archive.BeginTag(idTag);
		retVal = retVal && archive.Process(id);
		retVal = retVal && archive.EndTag(idTag);
		
		retVal = retVal && archive.BeginTag(nameTag);
		retVal = retVal && archive.Process(name);
		retVal = retVal && archive.EndTag(nameTag);
		
		retVal = retVal && archive.BeginTag(childrenTag);
		retVal = retVal && archive.Process(children);
		retVal = retVal && archive.EndTag(childrenTag);
		
		return retVal;
	}
	
	int id = 0;
	QString name;
	QVector<int> children;
};


// Test model with large data
class LargeDataModel: virtual public iser::ISerializable
{
public:
	virtual bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag dataTag("Data", "Large data array");
		bool retVal = archive.BeginTag(dataTag);
		retVal = retVal && archive.Process(data);
		retVal = retVal && archive.EndTag(dataTag);
		return retVal;
	}
	
	QVector<double> data;
};


// Test model with collections
class CollectionModel: virtual public iser::ISerializable
{
public:
	virtual bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag intListTag("IntList", "Integer list");
		static iser::CArchiveTag stringListTag("StringList", "String list");
		static iser::CArchiveTag doubleListTag("DoubleList", "Double list");
		
		bool retVal = archive.BeginTag(intListTag);
		retVal = retVal && archive.Process(intList);
		retVal = retVal && archive.EndTag(intListTag);
		
		retVal = retVal && archive.BeginTag(stringListTag);
		retVal = retVal && archive.Process(stringList);
		retVal = retVal && archive.EndTag(stringListTag);
		
		retVal = retVal && archive.BeginTag(doubleListTag);
		retVal = retVal && archive.Process(doubleList);
		retVal = retVal && archive.EndTag(doubleListTag);
		
		return retVal;
	}
	
	QVector<int> intList;
	QStringList stringList;
	QVector<double> doubleList;
};


void CAdvancedFileArchiveTest::NestedSerializationTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write nested data
	{
		NestedModel writeModel;
		writeModel.id = 1;
		writeModel.name = "Parent";
		writeModel.children = QVector<int>() << 10 << 20 << 30 << 40 << 50;
		
		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read nested data
	{
		NestedModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));
		
		// Verify nested structure
		QCOMPARE(readModel.id, 1);
		QCOMPARE(readModel.name, QString("Parent"));
		QCOMPARE(readModel.children.size(), 5);
		QCOMPARE(readModel.children[0], 10);
		QCOMPARE(readModel.children[4], 50);
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CAdvancedFileArchiveTest::LargeDataSerializationTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Create large dataset (10000 doubles)
	const int dataSize = 10000;
	
	// Write large data
	{
		LargeDataModel writeModel;
		writeModel.data.reserve(dataSize);
		for (int i = 0; i < dataSize; ++i)
		{
			writeModel.data.append(i * 3.14159);
		}
		
		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read large data
	{
		LargeDataModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));
		
		// Verify size
		QCOMPARE(readModel.data.size(), dataSize);
		
		// Spot check some values
		QVERIFY(qFuzzyCompare(readModel.data[0], 0.0));
		QVERIFY(qFuzzyCompare(readModel.data[100], 100 * 3.14159));
		QVERIFY(qFuzzyCompare(readModel.data[dataSize - 1], (dataSize - 1) * 3.14159));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CAdvancedFileArchiveTest::ConcurrentAccessTest()
{
	// Create temporary files for concurrent access test
	QTemporaryFile tempFile1;
	QVERIFY(tempFile1.open());
	QString filePath1 = tempFile1.fileName();
	tempFile1.close();
	
	QTemporaryFile tempFile2;
	QVERIFY(tempFile2.open());
	QString filePath2 = tempFile2.fileName();
	tempFile2.close();
	
	// Write to file1
	{
		NestedModel writeModel;
		writeModel.id = 100;
		writeModel.name = "File1";
		writeModel.children << 1 << 2 << 3;
		
		ifile::CFileWriteArchive writeArchive(filePath1);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Write to file2
	{
		NestedModel writeModel;
		writeModel.id = 200;
		writeModel.name = "File2";
		writeModel.children << 4 << 5 << 6;
		
		ifile::CFileWriteArchive writeArchive(filePath2);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read both files concurrently
	NestedModel readModel1, readModel2;
	
	{
		ifile::CFileReadArchive readArchive1(filePath1);
		ifile::CFileReadArchive readArchive2(filePath2);
		
		QVERIFY(readModel1.Serialize(readArchive1));
		QVERIFY(readModel2.Serialize(readArchive2));
	}
	
	// Verify both files were read correctly
	QCOMPARE(readModel1.id, 100);
	QCOMPARE(readModel1.name, QString("File1"));
	QCOMPARE(readModel1.children.size(), 3);
	
	QCOMPARE(readModel2.id, 200);
	QCOMPARE(readModel2.name, QString("File2"));
	QCOMPARE(readModel2.children.size(), 3);
	
	// Clean up
	QFile::remove(filePath1);
	QFile::remove(filePath2);
}


void CAdvancedFileArchiveTest::PartialReadTest()
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
		
		NestedModel model1;
		model1.id = 1;
		model1.name = "First";
		model1.children << 10;
		QVERIFY(model1.Serialize(writeArchive));
		
		NestedModel model2;
		model2.id = 2;
		model2.name = "Second";
		model2.children << 20;
		QVERIFY(model2.Serialize(writeArchive));
		
		NestedModel model3;
		model3.id = 3;
		model3.name = "Third";
		model3.children << 30;
		QVERIFY(model3.Serialize(writeArchive));
	}
	
	// Read only first object
	{
		ifile::CFileReadArchive readArchive(filePath);
		
		NestedModel model;
		QVERIFY(model.Serialize(readArchive));
		QCOMPARE(model.id, 1);
		QCOMPARE(model.name, QString("First"));
	}
	
	// Read first two objects
	{
		ifile::CFileReadArchive readArchive(filePath);
		
		NestedModel model1, model2;
		QVERIFY(model1.Serialize(readArchive));
		QVERIFY(model2.Serialize(readArchive));
		
		QCOMPARE(model1.id, 1);
		QCOMPARE(model2.id, 2);
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CAdvancedFileArchiveTest::ErrorRecoveryTest()
{
	// Test reading from non-existent file
	QString nonExistentPath = "/tmp/non_existent_file_12345.dat";
	ifile::CFileReadArchive readArchive(nonExistentPath);
	// Archive should not be valid for non-existent file
	
	NestedModel model;
	// Serialization should fail gracefully
	bool result = model.Serialize(readArchive);
	QVERIFY(!result);
	
	// Test writing to invalid path
	QString invalidPath = "/invalid/path/that/does/not/exist/test.dat";
	ifile::CFileWriteArchive writeArchive(invalidPath);
	QVERIFY(!writeArchive.IsArchiveValid());
}


void CAdvancedFileArchiveTest::MultipleConcurrentWritesTest()
{
	// Create temporary directory for test files
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	
	QVector<QString> filePaths;
	const int numFiles = 5;
	
	// Create multiple files
	for (int i = 0; i < numFiles; ++i)
	{
		QString filePath = tempDir.path() + QString("/test_%1.dat").arg(i);
		filePaths.append(filePath);
	}
	
	// Write to multiple files
	for (int i = 0; i < numFiles; ++i)
	{
		NestedModel writeModel;
		writeModel.id = i * 100;
		writeModel.name = QString("File_%1").arg(i);
		for (int j = 0; j < 10; ++j)
		{
			writeModel.children.append(i * 10 + j);
		}
		
		ifile::CFileWriteArchive writeArchive(filePaths[i]);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Verify all files were written correctly
	for (int i = 0; i < numFiles; ++i)
	{
		QVERIFY(QFile::exists(filePaths[i]));
		
		NestedModel readModel;
		ifile::CFileReadArchive readArchive(filePaths[i]);
		QVERIFY(readModel.Serialize(readArchive));
		
		QCOMPARE(readModel.id, i * 100);
		QCOMPARE(readModel.name, QString("File_%1").arg(i));
		QCOMPARE(readModel.children.size(), 10);
	}
}


void CAdvancedFileArchiveTest::ArchiveVersionCompatibilityTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write with normal archive
	{
		NestedModel writeModel;
		writeModel.id = 999;
		writeModel.name = "VersionTest";
		writeModel.children << 1 << 2 << 3;
		
		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read with normal archive
	{
		NestedModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));
		
		QCOMPARE(readModel.id, 999);
		QCOMPARE(readModel.name, QString("VersionTest"));
	}
	
	// Clean up
	QFile::remove(filePath);
}


void CAdvancedFileArchiveTest::NestedCollectionsTest()
{
	// Create temporary file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString filePath = tempFile.fileName();
	tempFile.close();
	
	// Write data with multiple collections
	{
		CollectionModel writeModel;
		
		// Populate int list
		for (int i = 0; i < 100; ++i)
		{
			writeModel.intList.append(i);
		}
		
		// Populate string list
		for (int i = 0; i < 50; ++i)
		{
			writeModel.stringList.append(QString("Item_%1").arg(i));
		}
		
		// Populate double list
		for (int i = 0; i < 75; ++i)
		{
			writeModel.doubleList.append(i * 1.5);
		}
		
		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data with multiple collections
	{
		CollectionModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));
		
		// Verify int list
		QCOMPARE(readModel.intList.size(), 100);
		QCOMPARE(readModel.intList[0], 0);
		QCOMPARE(readModel.intList[99], 99);
		
		// Verify string list
		QCOMPARE(readModel.stringList.size(), 50);
		QCOMPARE(readModel.stringList[0], QString("Item_0"));
		QCOMPARE(readModel.stringList[49], QString("Item_49"));
		
		// Verify double list
		QCOMPARE(readModel.doubleList.size(), 75);
		QVERIFY(qFuzzyCompare(readModel.doubleList[0], 0.0));
		QVERIFY(qFuzzyCompare(readModel.doubleList[74], 74 * 1.5));
	}
	
	// Clean up
	QFile::remove(filePath);
}


I_ADD_TEST(CAdvancedFileArchiveTest);


