// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CFileComponentTestRunner.h"


// Qt includes
#include <QtCore/QTemporaryDir>
#include <QtCore/QFile>

// ACF includes
#include <icomp/CInstanceRegistry.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


void CFileComponentTestRunner::initTestCase()
{
	m_testPartituraInstanceCompPtr = std::make_shared<CFileComponentTest>();
	QVERIFY(m_testPartituraInstanceCompPtr != nullptr);
}


void CFileComponentTestRunner::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


// FileNameParam tests

void CFileComponentTestRunner::testFileNameParamCreation()
{
	// Get FileNameParam component
	ifile::IFileNameParam* fileNameParamPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("FileNameParam");
	
	QVERIFY(fileNameParamPtr != nullptr);
	QVERIFY(!fileNameParamPtr->GetPath().isEmpty());
}


void CFileComponentTestRunner::testFileNameParamPathType()
{
	// Test different path types
	ifile::IFileNameParam* fileParamPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("FileNameParamFile");
	QVERIFY(fileParamPtr != nullptr);
	QCOMPARE(fileParamPtr->GetPathType(), (int)ifile::IFileNameParam::PT_FILE);
	
	ifile::IFileNameParam* dirParamPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("FileNameParamDir");
	QVERIFY(dirParamPtr != nullptr);
	QCOMPARE(dirParamPtr->GetPathType(), (int)ifile::IFileNameParam::PT_DIRECTORY);
}


void CFileComponentTestRunner::testFileNameParamDefaultPath()
{
	// Test that default path is set
	ifile::IFileNameParam* fileNameParamPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("FileNameParam");
	
	QVERIFY(fileNameParamPtr != nullptr);
	QString path = fileNameParamPtr->GetPath();
	QVERIFY(!path.isEmpty());
	QCOMPARE(path, QString("."));
}


void CFileComponentTestRunner::testFileNameParamGetSet()
{
	// Test get/set operations
	ifile::IFileNameParam* fileNameParamPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("FileNameParam");
	
	QVERIFY(fileNameParamPtr != nullptr);
	
	QString testPath = "/test/path/file.txt";
	fileNameParamPtr->SetPath(testPath);
	QCOMPARE(fileNameParamPtr->GetPath(), testPath);
}


void CFileComponentTestRunner::testFileNameParamSerialization()
{
	// Test serialization
	ifile::IFileNameParam* fileNameParamPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("FileNameParam");
	
	QVERIFY(fileNameParamPtr != nullptr);
	
	// Set a test path
	QString testPath = "/test/serialization/path.txt";
	fileNameParamPtr->SetPath(testPath);
	
	// Serialize
	iser::CMemoryWriteArchive writeArchive;
	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(fileNameParamPtr);
	QVERIFY(serializablePtr != nullptr);
	QVERIFY(serializablePtr->Serialize(writeArchive));
	
	// Reset path
	fileNameParamPtr->SetPath("");
	QVERIFY(fileNameParamPtr->GetPath().isEmpty());
	
	// Deserialize
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(serializablePtr->Serialize(readArchive));
	
	// Verify path was restored
	QCOMPARE(fileNameParamPtr->GetPath(), testPath);
}


// FileTypeInfo tests

void CFileComponentTestRunner::testFileTypeInfoCreation()
{
	// Get FileTypeInfo component
	ifile::IFileTypeInfo* fileTypeInfoPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileTypeInfo>("FileTypeInfo");
	
	QVERIFY(fileTypeInfoPtr != nullptr);
}


void CFileComponentTestRunner::testFileTypeInfoExtensions()
{
	// Test getting file extensions
	ifile::IFileTypeInfo* fileTypeInfoPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileTypeInfo>("FileTypeInfo");
	
	QVERIFY(fileTypeInfoPtr != nullptr);
	
	QStringList extensions;
	QVERIFY(fileTypeInfoPtr->GetFileExtensions(extensions));
	QVERIFY(!extensions.isEmpty());
	QVERIFY(extensions.contains("txt"));
	QVERIFY(extensions.contains("log"));
	QVERIFY(extensions.contains("md"));
}


void CFileComponentTestRunner::testFileTypeInfoDescriptions()
{
	// Test getting type descriptions
	ifile::IFileTypeInfo* fileTypeInfoPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileTypeInfo>("FileTypeInfo");
	
	QVERIFY(fileTypeInfoPtr != nullptr);
	
	QString description = fileTypeInfoPtr->GetTypeDescription();
	QVERIFY(!description.isEmpty());
}


void CFileComponentTestRunner::testFileTypeInfoPlatformSpecific()
{
	// Test platform-specific extensions
	ifile::IFileTypeInfo* fileTypeInfoPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileTypeInfo>("FileTypeInfoPlatform");
	
	QVERIFY(fileTypeInfoPtr != nullptr);
	
	QStringList extensions;
	QVERIFY(fileTypeInfoPtr->GetFileExtensions(extensions));
	QVERIFY(!extensions.isEmpty());
	
	// Basic extensions should be present
	QVERIFY(extensions.contains("dat"));
	QVERIFY(extensions.contains("bin"));
	
	// Platform-specific extensions will be added based on the current platform
#ifdef Q_OS_WIN
	QVERIFY(extensions.contains("exe") || extensions.contains("dll"));
#elif defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
	QVERIFY(extensions.contains("so") || extensions.contains("a"));
#endif
}


// TempFileManager tests

void CFileComponentTestRunner::testTempFileManagerCreation()
{
	// Get TempFileManager component
	ifile::ITempFileManager* tempFileManagerPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::ITempFileManager>("TempFileManager");
	
	QVERIFY(tempFileManagerPtr != nullptr);
}


void CFileComponentTestRunner::testTempFileManagerSession()
{
	// Test creating a session
	ifile::ITempFileManager* tempFileManagerPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::ITempFileManager>("TempFileManager");
	
	QVERIFY(tempFileManagerPtr != nullptr);
	
	QByteArray sessionId = tempFileManagerPtr->BeginSession("test_session");
	QVERIFY(!sessionId.isEmpty());
	
	// Finish session
	tempFileManagerPtr->FinishSession(sessionId);
}


void CFileComponentTestRunner::testTempFileManagerAddFile()
{
	// Test adding a file to session
	ifile::ITempFileManager* tempFileManagerPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::ITempFileManager>("TempFileManager");
	
	QVERIFY(tempFileManagerPtr != nullptr);
	
	QByteArray sessionId = tempFileManagerPtr->BeginSession("test_add_file");
	QVERIFY(!sessionId.isEmpty());
	
	// Add a file
	QByteArray fileId = tempFileManagerPtr->AddFileItem(sessionId, "test_file.txt");
	QVERIFY(!fileId.isEmpty());
	
	// Get file path
	QString filePath = tempFileManagerPtr->GetPath(sessionId, fileId);
	QVERIFY(!filePath.isEmpty());
	
	// Finish session
	tempFileManagerPtr->FinishSession(sessionId);
}


void CFileComponentTestRunner::testTempFileManagerRemoveFile()
{
	// Test removing a file from session
	ifile::ITempFileManager* tempFileManagerPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::ITempFileManager>("TempFileManager");
	
	QVERIFY(tempFileManagerPtr != nullptr);
	
	QByteArray sessionId = tempFileManagerPtr->BeginSession("test_remove_file");
	QVERIFY(!sessionId.isEmpty());
	
	// Add a file
	QByteArray fileId = tempFileManagerPtr->AddFileItem(sessionId, "test_file_to_remove.txt");
	QVERIFY(!fileId.isEmpty());
	
	// Remove the file
	QVERIFY(tempFileManagerPtr->RemoveFileItem(sessionId, fileId));
	
	// Verify file path is no longer available
	QString filePath = tempFileManagerPtr->GetPath(sessionId, fileId);
	QVERIFY(filePath.isEmpty());
	
	// Finish session
	tempFileManagerPtr->FinishSession(sessionId);
}


void CFileComponentTestRunner::testTempFileManagerMultipleSessions()
{
	// Test multiple concurrent sessions
	ifile::ITempFileManager* tempFileManagerPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::ITempFileManager>("TempFileManager");
	
	QVERIFY(tempFileManagerPtr != nullptr);
	
	QByteArray session1 = tempFileManagerPtr->BeginSession("session1");
	QByteArray session2 = tempFileManagerPtr->BeginSession("session2");
	
	QVERIFY(!session1.isEmpty());
	QVERIFY(!session2.isEmpty());
	QVERIFY(session1 != session2);
	
	// Add files to different sessions
	QByteArray file1 = tempFileManagerPtr->AddFileItem(session1, "file1.txt");
	QByteArray file2 = tempFileManagerPtr->AddFileItem(session2, "file2.txt");
	
	QVERIFY(!file1.isEmpty());
	QVERIFY(!file2.isEmpty());
	
	// Verify files are in correct sessions
	QString path1 = tempFileManagerPtr->GetPath(session1, file1);
	QString path2 = tempFileManagerPtr->GetPath(session2, file2);
	
	QVERIFY(!path1.isEmpty());
	QVERIFY(!path2.isEmpty());
	QVERIFY(path1 != path2);
	
	// Finish sessions
	tempFileManagerPtr->FinishSession(session1);
	tempFileManagerPtr->FinishSession(session2);
}


// SystemLocation tests

void CFileComponentTestRunner::testSystemLocationCreation()
{
	// Get SystemLocation component
	ifile::IFileNameParam* systemLocationPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("SystemLocation");
	
	QVERIFY(systemLocationPtr != nullptr);
}


void CFileComponentTestRunner::testSystemLocationPath()
{
	// Test that system location returns a valid path
	ifile::IFileNameParam* systemLocationPtr = 
		icomp::CInstanceRegistry::GetInstance().GetTypedObject<ifile::IFileNameParam>("SystemLocation");
	
	QVERIFY(systemLocationPtr != nullptr);
	
	QString path = systemLocationPtr->GetPath();
	QVERIFY(!path.isEmpty());
}


I_ADD_TEST(CFileComponentTestRunner);


