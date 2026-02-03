// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CSystem.h>
#include <itest/CStandardTestExecutor.h>

class CSystemTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetOperationSystemNameTest();
	void ConvertToFileNameTest();
	void ConvertToFileNameWithReplacementTest();
	void ConvertToFileNameInvalidReplacementTest();
	void ConvertToFileNameEmptyInputTest();
	void GetNormalizedPathTest();
	void GetVariableValueTest();
	void GetVariableValueEmbeddedTest();
	void GetVariableValueEnvironmentTest();
	void GetEnrolledPathTest();
	void GetEnrolledPathMultipleVariablesTest();
	void GetEnvironmentVariablesTest();
	void SetTempDirectoryPathTest();
	void SleepTest();
	void CopyDirectoryTest();
	void RemoveDirectoryTest();
	void FileCopyTest();
	void FileCopyOverwriteTest();
	void FileCopyNonExistentSourceTest();
	void FileMoveTest();
	void FileMoveOverwriteTest();
	void EnsurePathExistsTest();
	void GetCurrentUserNameTest();
	void GetFileDriveInfoTest();
	void GetCompilerVariableTest();
	void SetUserVariablesTest();
	void ConvertNetworkPathToUncTest();
	void GetCompilerInfoTest();

	void cleanupTestCase();

private:
	QString m_testDir;
};


