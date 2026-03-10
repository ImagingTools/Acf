// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <ifile/IFileNameParam.h>
#include <ifile/IFileTypeInfo.h>
#include <ifile/ITempFileManager.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/FileComponentTest/CFileComponentTest.h>


class CFileComponentTestRunner: public QObject
{
	Q_OBJECT
	
private slots:
	void initTestCase();
	
	// FileNameParam tests
	void testFileNameParamCreation();
	void testFileNameParamPathType();
	void testFileNameParamDefaultPath();
	void testFileNameParamGetSet();
	void testFileNameParamSerialization();
	
	// FileTypeInfo tests
	void testFileTypeInfoCreation();
	void testFileTypeInfoExtensions();
	void testFileTypeInfoDescriptions();
	void testFileTypeInfoPlatformSpecific();
	
	// TempFileManager tests
	void testTempFileManagerCreation();
	void testTempFileManagerSession();
	void testTempFileManagerAddFile();
	void testTempFileManagerRemoveFile();
	void testTempFileManagerMultipleSessions();
	
	// SystemLocation tests
	void testSystemLocationCreation();
	void testSystemLocationPath();
	
	// FileListProvider tests
	void testFileListProviderCreation();
	void testFileListProviderGetFileList();
	void testFileListProviderStaticMethods();
	
	// ComposedFilePersistence tests
	void testComposedPersistenceCreation();
	void testComposedPersistenceExtensions();
	void testComposedPersistenceDescription();
	
	void cleanupTestCase();
	
private:
	std::shared_ptr<CFileComponentTest> m_testPartituraInstanceCompPtr;
};


