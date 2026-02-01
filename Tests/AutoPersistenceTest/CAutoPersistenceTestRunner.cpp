// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CAutoPersistenceTestRunner.h"


// Qt includes
#include <QtCore/QThread>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

// ACF includes
#include <iprm/ISelectionParam.h>


// private slots

void CAutoPersistenceTestRunner::initTestCase()
{
}


QString CAutoPersistenceTestRunner::CreateTestFilePath(const QString& testName)
{
	QString tempDir = QDir::tempPath();
	QString filePath = tempDir + "/" + testName + "_test.xml";
	
	// Clean up any existing file
	if (QFile::exists(filePath)){
		QFile::remove(filePath);
	}
	
	return filePath;
}


void CAutoPersistenceTestRunner::CleanupTestFile(const QString& filePath)
{
	if (QFile::exists(filePath)){
		QFile::remove(filePath);
	}
	
	// Also cleanup lock file if exists
	QString lockFilePath = filePath + ".lock";
	if (QFile::exists(lockFilePath)){
		QFile::remove(lockFilePath);
	}
}


void CAutoPersistenceTestRunner::RestoreOnBeginStoreOnEndTest()
{
	QString filePath = CreateTestFilePath("RestoreOnBeginStoreOnEnd");
	
	// First run: Create object, modify it, and let it store on end
	{
		CAutoPersistenceTest test;
		test.EnsureAutoInitComponentsCreated();
		
		ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathRestoreStoreEnd");
		QVERIFY(filePathPtr != nullptr);
		filePathPtr->SetPath(filePath);
		
		iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionRestoreStoreEnd");
		QVERIFY(selectionPtr != nullptr);
		
		// Set initial value
		selectionPtr->SetSelectedOptionIndex(5);
		QCOMPARE(selectionPtr->GetSelectedOptionIndex(), 5);
		
		// File should not exist yet (StoreOnEnd is enabled but component hasn't destroyed)
	}
	// Component destroyed, should have stored to file
	
	// Wait a bit for async operations to complete
	QThread::msleep(100);
	QCoreApplication::processEvents();
	
	// Verify file was created
	QVERIFY(QFile::exists(filePath));
	
	// Second run: Restore from file
	{
		CAutoPersistenceTest test;
		test.EnsureAutoInitComponentsCreated();
		
		ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathRestoreStoreEnd");
		QVERIFY(filePathPtr != nullptr);
		filePathPtr->SetPath(filePath);
		
		// Wait for restoration to complete
		QThread::msleep(100);
		QCoreApplication::processEvents();
		
		iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionRestoreStoreEnd");
		QVERIFY(selectionPtr != nullptr);
		
		// Verify value was restored
		QCOMPARE(selectionPtr->GetSelectedOptionIndex(), 5);
	}
	
	CleanupTestFile(filePath);
}


void CAutoPersistenceTestRunner::StoreOnChangeTest()
{
	QString filePath = CreateTestFilePath("StoreOnChange");
	
	CAutoPersistenceTest test;
	test.EnsureAutoInitComponentsCreated();
	
	ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathStoreOnChange");
	QVERIFY(filePathPtr != nullptr);
	filePathPtr->SetPath(filePath);
	
	iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionStoreOnChange");
	QVERIFY(selectionPtr != nullptr);
	
	// Set initial value - should trigger immediate save
	selectionPtr->SetSelectedOptionIndex(3);
	
	// Wait for save operation to complete
	QThread::msleep(200);
	QCoreApplication::processEvents();
	
	// Verify file was created
	QVERIFY(QFile::exists(filePath));
	
	// Modify value again - should trigger another save
	selectionPtr->SetSelectedOptionIndex(7);
	
	QThread::msleep(200);
	QCoreApplication::processEvents();
	
	CleanupTestFile(filePath);
}


void CAutoPersistenceTestRunner::StoreOnBeginTest()
{
	QString filePath = CreateTestFilePath("StoreOnBegin");
	
	// Ensure file doesn't exist
	CleanupTestFile(filePath);
	
	CAutoPersistenceTest test;
	
	// Set file path BEFORE initialization
	ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathStoreOnBegin");
	QVERIFY(filePathPtr != nullptr);
	filePathPtr->SetPath(filePath);
	
	// Now initialize components - StoreOnBegin should trigger now
	test.EnsureAutoInitComponentsCreated();
	
	// Wait for store operation to complete
	QThread::msleep(200);
	QCoreApplication::processEvents();
	
	// Verify file was created on begin (since it didn't exist)
	QVERIFY(QFile::exists(filePath));
	
	CleanupTestFile(filePath);
}


void CAutoPersistenceTestRunner::StoreIntervalTest()
{
	QString filePath = CreateTestFilePath("StoreInterval");
	
	CAutoPersistenceTest test;
	test.EnsureAutoInitComponentsCreated();
	
	ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathStoreInterval");
	QVERIFY(filePathPtr != nullptr);
	filePathPtr->SetPath(filePath);
	
	iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionStoreInterval");
	QVERIFY(selectionPtr != nullptr);
	
	// Set a value - with StoreInterval, it should not save immediately
	selectionPtr->SetSelectedOptionIndex(4);
	
	// Wait less than the interval
	QThread::msleep(500);
	QCoreApplication::processEvents();
	
	// File may or may not exist yet depending on interval timing
	// The main test is that the component doesn't crash
	
	// Wait for the interval to trigger (configured as 1 second)
	QThread::msleep(1500);
	QCoreApplication::processEvents();
	
	// Now file should exist
	QVERIFY(QFile::exists(filePath));
	
	CleanupTestFile(filePath);
}


void CAutoPersistenceTestRunner::AutoReloadTest()
{
	QString filePath = CreateTestFilePath("AutoReload");
	
	// First, create a file with some data
	{
		CAutoPersistenceTest test;
		test.EnsureAutoInitComponentsCreated();
		
		ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathAutoReload");
		QVERIFY(filePathPtr != nullptr);
		filePathPtr->SetPath(filePath);
		
		iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionAutoReload");
		QVERIFY(selectionPtr != nullptr);
		
		selectionPtr->SetSelectedOptionIndex(2);
		
		// Wait for store
		QThread::msleep(200);
		QCoreApplication::processEvents();
	}
	
	QVERIFY(QFile::exists(filePath));
	
	// Create another instance with AutoReload enabled
	{
		CAutoPersistenceTest test;
		test.EnsureAutoInitComponentsCreated();
		
		ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathAutoReload");
		QVERIFY(filePathPtr != nullptr);
		filePathPtr->SetPath(filePath);
		
		QThread::msleep(200);
		QCoreApplication::processEvents();
		
		iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionAutoReload");
		QVERIFY(selectionPtr != nullptr);
		
		// Verify it was loaded
		QCOMPARE(selectionPtr->GetSelectedOptionIndex(), 2);
		
		// Note: Actually testing auto-reload would require modifying the file externally
		// which is complex in a unit test. This test mainly verifies the component
		// initializes correctly with AutoReload enabled.
	}
	
	CleanupTestFile(filePath);
}


void CAutoPersistenceTestRunner::LockingTest()
{
	QString filePath = CreateTestFilePath("Locking");
	
	// Test with lock file attributes enabled
	CAutoPersistenceTest test;
	test.EnsureAutoInitComponentsCreated();
	
	ifile::IFileNameParam* filePathPtr = test.GetInterface<ifile::IFileNameParam>("FilePathLocking");
	QVERIFY(filePathPtr != nullptr);
	filePathPtr->SetPath(filePath);
	
	iprm::ISelectionParam* selectionPtr = test.GetInterface<iprm::ISelectionParam>("SelectionLocking");
	QVERIFY(selectionPtr != nullptr);
	
	// Set value and trigger save
	selectionPtr->SetSelectedOptionIndex(8);
	
	QThread::msleep(200);
	QCoreApplication::processEvents();
	
	// Verify file was created
	QVERIFY(QFile::exists(filePath));
	
	CleanupTestFile(filePath);
}


void CAutoPersistenceTestRunner::cleanupTestCase()
{
}


I_ADD_TEST(CAutoPersistenceTestRunner);
