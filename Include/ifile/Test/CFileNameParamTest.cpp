// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CFileNameParamTest.h>


// Qt includes
#include <QtCore/QTemporaryDir>
#include <QtCore/QTemporaryFile>


void CFileNameParamTest::GetSetPathTest()
{
	ifile::CFileNameParam param;
	
	// Test setting and getting path
	QString testPath = "/home/user/test.txt";
	param.SetPath(testPath);
	QCOMPARE(param.GetPath(), testPath);
	
	// Test changing path
	QString newPath = "/home/user/other.txt";
	param.SetPath(newPath);
	QCOMPARE(param.GetPath(), newPath);
}


void CFileNameParamTest::PathTypeTest()
{
	ifile::CFileNameParam param;
	
	// Create temporary file and directory for testing
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString tempFilePath = tempFile.fileName();
	tempFile.close();
	
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	QString tempDirPath = tempDir.path();
	
	// Test file path type
	param.SetPath(tempFilePath);
	QCOMPARE(param.GetPathType(), (int)ifile::IFileNameParam::PT_FILE);
	
	// Test directory path type
	param.SetPath(tempDirPath);
	QCOMPARE(param.GetPathType(), (int)ifile::IFileNameParam::PT_DIRECTORY);
	
	// Test unknown path type (non-existent path)
	param.SetPath("/non/existent/path.txt");
	QCOMPARE(param.GetPathType(), (int)ifile::IFileNameParam::PT_UNKNOWN);
}


void CFileNameParamTest::SerializationTest()
{
	// Create and set path
	ifile::CFileNameParam writeParam;
	QString testPath = "/home/user/documents/test.txt";
	writeParam.SetPath(testPath);
	
	// Serialize to memory
	iser::CMemoryWriteArchive writeArchive;
	QVERIFY(writeParam.Serialize(writeArchive));
	
	// Deserialize from memory
	ifile::CFileNameParam readParam;
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readParam.Serialize(readArchive));
	
	// Verify path was restored
	QCOMPARE(readParam.GetPath(), testPath);
}


void CFileNameParamTest::CopyFromTest()
{
	ifile::CFileNameParam source;
	ifile::CFileNameParam dest;
	
	QString testPath = "/home/user/source.txt";
	source.SetPath(testPath);
	
	// Copy from source to dest
	QVERIFY(dest.CopyFrom(source));
	QCOMPARE(dest.GetPath(), testPath);
	
	// Test copying empty path
	ifile::CFileNameParam emptySource;
	ifile::CFileNameParam emptyDest;
	emptyDest.SetPath("/some/path.txt");
	QVERIFY(emptyDest.CopyFrom(emptySource));
	QCOMPARE(emptyDest.GetPath(), QString());
}


void CFileNameParamTest::IsEqualTest()
{
	ifile::CFileNameParam param1;
	ifile::CFileNameParam param2;
	
	// Test empty paths are equal
	QVERIFY(param1.IsEqual(param2));
	
	// Test same paths are equal
	QString testPath = "/home/user/test.txt";
	param1.SetPath(testPath);
	param2.SetPath(testPath);
	QVERIFY(param1.IsEqual(param2));
	
	// Test different paths are not equal
	param2.SetPath("/home/user/other.txt");
	QVERIFY(!param1.IsEqual(param2));
	
	// Test relative vs absolute paths that resolve to same file
	QTemporaryFile tempFile;
	QVERIFY(tempFile.open());
	QString absolutePath = tempFile.fileName();
	tempFile.close();
	
	param1.SetPath(absolutePath);
	param2.SetPath(absolutePath);
	QVERIFY(param1.IsEqual(param2));
}


void CFileNameParamTest::EmptyPathTest()
{
	ifile::CFileNameParam param;
	
	// Default path should be empty
	QVERIFY(param.GetPath().isEmpty());
	
	// Set empty path
	param.SetPath(QString());
	QVERIFY(param.GetPath().isEmpty());
	
	// PathType should be unknown for empty path
	QCOMPARE(param.GetPathType(), (int)ifile::IFileNameParam::PT_UNKNOWN);
}


void CFileNameParamTest::SupportedOperationsTest()
{
	ifile::CFileNameParam param;
	
	int supportedOps = param.GetSupportedOperations();
	
	// Should support copy and compare operations
	QVERIFY(supportedOps & istd::IChangeable::SO_COPY);
	QVERIFY(supportedOps & istd::IChangeable::SO_COMPARE);
}


I_ADD_TEST(CFileNameParamTest);


