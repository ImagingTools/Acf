// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/ISelectionParam.h>
#include <ifile/IFileNameParam.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/AutoPersistenceTest/CAutoPersistenceTest.h>

class CAutoPersistenceTestRunner: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void RestoreOnBeginStoreOnEndTest();
	void StoreOnChangeTest();
	void StoreOnBeginTest();
	void StoreIntervalTest();
	void AutoReloadTest();
	void LockingTest();

	void cleanupTestCase();

private:
	QString CreateTestFilePath(const QString& testName);
	void CleanupTestFile(const QString& filePath);
};

