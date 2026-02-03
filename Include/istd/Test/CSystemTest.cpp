// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CSystemTest.h"


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>


// protected slots

void CSystemTest::initTestCase()
{
	// Create a temporary test directory for file operations
	QString tempPath = QDir::tempPath();
	m_testDir = tempPath + "/CSystemTest_" + QString::number(QDateTime::currentMSecsSinceEpoch());
	
	QDir dir;
	bool created = dir.mkpath(m_testDir);
	QVERIFY(created);
	QVERIFY(QDir(m_testDir).exists());
}


void CSystemTest::GetOperationSystemNameTest()
{
	QString osName = istd::CSystem::GetOperationSystemName();
	
	// OS name should not be empty
	QVERIFY(!osName.isEmpty());
	
	// Should be one of the supported OS names
	bool isValid = (osName == "Windows" || osName == "OSX" || osName == "iOS" || 
					osName == "Linux" || osName == "Unix");
	QVERIFY(isValid);
}


void CSystemTest::ConvertToFileNameTest()
{
	QString result;
	
	// Test with valid file name (no changes needed)
	bool success = istd::CSystem::ConvertToFileName("ValidFileName.txt", result);
	QVERIFY(success);
	QCOMPARE(result, QString("ValidFileName.txt"));
	
	// Test with reserved characters (should be removed)
	success = istd::CSystem::ConvertToFileName("Invalid*Name<>File|Name.txt", result);
	QVERIFY(success);
	QVERIFY(!result.contains("*"));
	QVERIFY(!result.contains("<"));
	QVERIFY(!result.contains(">"));
	QVERIFY(!result.contains("|"));
}


void CSystemTest::ConvertToFileNameWithReplacementTest()
{
	QString result;
	
	// Test with underscore replacement
	bool success = istd::CSystem::ConvertToFileName("Test*File<Name>.txt", result, "_");
	QVERIFY(success);
	QCOMPARE(result, QString("Test_File_Name_.txt"));
	
	// Test with dash replacement
	success = istd::CSystem::ConvertToFileName("Test:File?Name.txt", result, "-");
	QVERIFY(success);
	QCOMPARE(result, QString("Test-File-Name.txt"));
}


void CSystemTest::ConvertToFileNameInvalidReplacementTest()
{
	QString result;
	
	// Test with invalid replacement character (contains reserved character)
	bool success = istd::CSystem::ConvertToFileName("TestFile.txt", result, "*");
	QVERIFY(!success);
	
	// Test with another invalid replacement
	success = istd::CSystem::ConvertToFileName("TestFile.txt", result, "<>");
	QVERIFY(!success);
}


void CSystemTest::ConvertToFileNameEmptyInputTest()
{
	QString result;
	
	// Test with empty input
	bool success = istd::CSystem::ConvertToFileName("", result);
	QVERIFY(!success);
}


void CSystemTest::GetNormalizedPathTest()
{
	// Test normalization with current directory
	QString path = istd::CSystem::GetNormalizedPath(".");
	QVERIFY(!path.isEmpty());
	QVERIFY(QDir(path).exists());
	
	// Test with absolute path
	QString testPath = m_testDir;
	QString normalized = istd::CSystem::GetNormalizedPath(testPath);
	QVERIFY(!normalized.isEmpty());
}


void CSystemTest::GetVariableValueTest()
{
	// Test ApplicationDir variable
	QString appDir = istd::CSystem::GetVariableValue("ApplicationDir");
	QVERIFY(!appDir.isEmpty());
	QCOMPARE(appDir, QCoreApplication::applicationDirPath());
	
	// Test current directory variable
	QString currentDir = istd::CSystem::GetVariableValue(".");
	QVERIFY(!currentDir.isEmpty());
	QCOMPARE(currentDir, QDir::currentPath());
}


void CSystemTest::GetVariableValueEmbeddedTest()
{
	// Set user variables
	istd::CSystem::SetUserVariables("Debug", "Qt6", "GCC", "x64");
	
	// Test PlatformCode variable
	QString platformCode = istd::CSystem::GetVariableValue("PlatformCode");
	QCOMPARE(platformCode, QString("x64"));
	
	// Test CompileMode variable
	QString compileMode = istd::CSystem::GetVariableValue("CompileMode");
	QCOMPARE(compileMode, QString("Debug"));
	
	// Test CompilerName variable
	QString compilerName = istd::CSystem::GetVariableValue("CompilerName");
	QCOMPARE(compilerName, QString("GCC"));
	
	// Test ConfigurationName variable
	QString configName = istd::CSystem::GetVariableValue("ConfigurationName");
	QVERIFY(configName.contains("Debug"));
	QVERIFY(configName.contains("Qt6"));
	QVERIFY(configName.contains("GCC"));
	QVERIFY(configName.contains("x64"));
	
	// Test ConfigurationDir variable
	QString configDir = istd::CSystem::GetVariableValue("ConfigurationDir");
	QVERIFY(!configDir.isEmpty());
	QVERIFY(configDir.endsWith("/"));
}


void CSystemTest::GetVariableValueEnvironmentTest()
{
	// Get environment variables
	istd::CSystem::EnvironmentVariables envVars = istd::CSystem::GetEnvironmentVariables();
	
	// If we have any environment variables, test one
	if (!envVars.isEmpty())
	{
		QString varName = envVars.keys().first();
		QString varValue = istd::CSystem::GetVariableValue(varName.toUpper(), true, false);
		QCOMPARE(varValue, envVars[varName]);
	}
}


void CSystemTest::GetEnrolledPathTest()
{
	// Set user variables for testing
	istd::CSystem::SetUserVariables("Release", "Qt6", "MSVC", "x64");
	
	// Test path with variable
	QString path = "$(ApplicationDir)/test.txt";
	QString enrolled = istd::CSystem::GetEnrolledPath(path);
	
	QVERIFY(!enrolled.isEmpty());
	QVERIFY(!enrolled.contains("$("));
	QVERIFY(!enrolled.contains(")"));
	QVERIFY(enrolled.contains(QCoreApplication::applicationDirPath()));
}


void CSystemTest::GetEnrolledPathMultipleVariablesTest()
{
	// Test path with multiple variables
	QString path = "$(ApplicationDir)/$(CompileMode)/data";
	QString enrolled = istd::CSystem::GetEnrolledPath(path);
	
	QVERIFY(!enrolled.isEmpty());
	QVERIFY(!enrolled.contains("$(CompileMode)"));
	QVERIFY(enrolled.contains(QCoreApplication::applicationDirPath()));
}


void CSystemTest::GetEnvironmentVariablesTest()
{
	istd::CSystem::EnvironmentVariables envVars = istd::CSystem::GetEnvironmentVariables();
	
	// Should have at least some environment variables
	QVERIFY(!envVars.isEmpty());
	
	// All keys should be uppercase
	for (const QString& key : envVars.keys())
	{
		QCOMPARE(key, key.toUpper());
	}
}


void CSystemTest::SetTempDirectoryPathTest()
{
	// Create a custom temp directory
	QString customTemp = m_testDir + "/CustomTemp";
	QDir dir;
	dir.mkpath(customTemp);
	
	// Set temp directory
	istd::CSystem::SetTempDirectoryPath(customTemp);
	
	// Verify it was set
	istd::CSystem::EnvironmentVariables envVars = istd::CSystem::GetEnvironmentVariables();
	QString tmpVar = envVars.value("TMP");
	QString tempVar = envVars.value("TEMP");
	
	// At least one should be set
	QVERIFY(tmpVar == customTemp || tempVar == customTemp);
}


void CSystemTest::SleepTest()
{
	// Test short sleep
	QDateTime before = QDateTime::currentDateTime();
	istd::CSystem::Sleep(0.1); // 100 milliseconds
	QDateTime after = QDateTime::currentDateTime();
	
	qint64 elapsed = before.msecsTo(after);
	
	// Should have slept at least 100ms (with some tolerance)
	QVERIFY(elapsed >= 90);
	QVERIFY(elapsed < 500); // Should not take too long
}


void CSystemTest::CopyDirectoryTest()
{
	// Create source directory with files
	QString sourceDir = m_testDir + "/SourceDir";
	QString destDir = m_testDir + "/DestDir";
	
	QDir dir;
	dir.mkpath(sourceDir);
	
	// Create test files
	QFile file1(sourceDir + "/file1.txt");
	file1.open(QIODevice::WriteOnly);
	file1.write("Test content 1");
	file1.close();
	
	QFile file2(sourceDir + "/file2.txt");
	file2.open(QIODevice::WriteOnly);
	file2.write("Test content 2");
	file2.close();
	
	// Create subdirectory
	dir.mkpath(sourceDir + "/SubDir");
	QFile file3(sourceDir + "/SubDir/file3.txt");
	file3.open(QIODevice::WriteOnly);
	file3.write("Test content 3");
	file3.close();
	
	// Copy directory
	bool success = istd::CSystem::CopyDirectory(sourceDir, destDir);
	QVERIFY(success);
	
	// Verify destination exists and has files
	QVERIFY(QDir(destDir).exists());
	QVERIFY(QFile::exists(destDir + "/file1.txt"));
	QVERIFY(QFile::exists(destDir + "/file2.txt"));
	QVERIFY(QFile::exists(destDir + "/SubDir/file3.txt"));
	
	// Verify content
	QFile destFile1(destDir + "/file1.txt");
	destFile1.open(QIODevice::ReadOnly);
	QString content1 = QString(destFile1.readAll());
	destFile1.close();
	QCOMPARE(content1, QString("Test content 1"));
}


void CSystemTest::RemoveDirectoryTest()
{
	// Create directory to remove
	QString dirToRemove = m_testDir + "/ToRemove";
	QDir dir;
	dir.mkpath(dirToRemove);
	
	// Create files and subdirectory
	QFile file(dirToRemove + "/test.txt");
	file.open(QIODevice::WriteOnly);
	file.write("Test");
	file.close();
	
	dir.mkpath(dirToRemove + "/SubDir");
	QFile subFile(dirToRemove + "/SubDir/subtest.txt");
	subFile.open(QIODevice::WriteOnly);
	subFile.write("Test");
	subFile.close();
	
	// Verify directory exists
	QVERIFY(QDir(dirToRemove).exists());
	
	// Remove directory
	bool success = istd::CSystem::RemoveDirectory(dirToRemove);
	QVERIFY(success);
	
	// Verify directory no longer exists
	QVERIFY(!QDir(dirToRemove).exists());
}


void CSystemTest::FileCopyTest()
{
	// Create source file
	QString sourceFile = m_testDir + "/source.txt";
	QString destFile = m_testDir + "/dest.txt";
	
	QFile file(sourceFile);
	file.open(QIODevice::WriteOnly);
	file.write("Test file content");
	file.close();
	
	// Copy file
	bool success = istd::CSystem::FileCopy(sourceFile, destFile);
	QVERIFY(success);
	
	// Verify destination exists
	QVERIFY(QFile::exists(destFile));
	
	// Verify content
	QFile destFileObj(destFile);
	destFileObj.open(QIODevice::ReadOnly);
	QString content = QString(destFileObj.readAll());
	destFileObj.close();
	QCOMPARE(content, QString("Test file content"));
}


void CSystemTest::FileCopyOverwriteTest()
{
	// Create source and destination files
	QString sourceFile = m_testDir + "/source2.txt";
	QString destFile = m_testDir + "/dest2.txt";
	
	QFile file(sourceFile);
	file.open(QIODevice::WriteOnly);
	file.write("New content");
	file.close();
	
	QFile destFileObj(destFile);
	destFileObj.open(QIODevice::WriteOnly);
	destFileObj.write("Old content");
	destFileObj.close();
	
	// Try to copy without overwrite (should fail)
	bool success = istd::CSystem::FileCopy(sourceFile, destFile, false);
	QVERIFY(!success);
	
	// Copy with overwrite
	success = istd::CSystem::FileCopy(sourceFile, destFile, true);
	QVERIFY(success);
	
	// Verify content was updated
	QFile verifyFile(destFile);
	verifyFile.open(QIODevice::ReadOnly);
	QString content = QString(verifyFile.readAll());
	verifyFile.close();
	QCOMPARE(content, QString("New content"));
}


void CSystemTest::FileCopyNonExistentSourceTest()
{
	// Try to copy non-existent file
	QString sourceFile = m_testDir + "/nonexistent.txt";
	QString destFile = m_testDir + "/dest3.txt";
	
	bool success = istd::CSystem::FileCopy(sourceFile, destFile);
	QVERIFY(!success);
}


void CSystemTest::FileMoveTest()
{
	// Create source file
	QString sourceFile = m_testDir + "/tomove.txt";
	QString targetFolder = m_testDir + "/MoveTarget";
	
	QFile file(sourceFile);
	file.open(QIODevice::WriteOnly);
	file.write("Move test content");
	file.close();
	
	// Create target folder
	QDir dir;
	dir.mkpath(targetFolder);
	
	// Move file
	bool success = istd::CSystem::FileMove(sourceFile, targetFolder);
	QVERIFY(success);
	
	// Verify source no longer exists
	QVERIFY(!QFile::exists(sourceFile));
	
	// Verify destination exists
	QString destFile = targetFolder + "/tomove.txt";
	QVERIFY(QFile::exists(destFile));
	
	// Verify content
	QFile destFileObj(destFile);
	destFileObj.open(QIODevice::ReadOnly);
	QString content = QString(destFileObj.readAll());
	destFileObj.close();
	QCOMPARE(content, QString("Move test content"));
}


void CSystemTest::FileMoveOverwriteTest()
{
	// Create source file
	QString sourceFile = m_testDir + "/tomove2.txt";
	QString targetFolder = m_testDir + "/MoveTarget2";
	
	QFile file(sourceFile);
	file.open(QIODevice::WriteOnly);
	file.write("New move content");
	file.close();
	
	// Create target folder and existing file
	QDir dir;
	dir.mkpath(targetFolder);
	
	QString destFile = targetFolder + "/tomove2.txt";
	QFile existingFile(destFile);
	existingFile.open(QIODevice::WriteOnly);
	existingFile.write("Old content");
	existingFile.close();
	
	// Try to move without overwrite (should fail)
	bool success = istd::CSystem::FileMove(sourceFile, targetFolder, false);
	QVERIFY(!success);
	
	// Verify source still exists
	QVERIFY(QFile::exists(sourceFile));
	
	// Move with overwrite
	success = istd::CSystem::FileMove(sourceFile, targetFolder, true);
	QVERIFY(success);
	
	// Verify source no longer exists
	QVERIFY(!QFile::exists(sourceFile));
	
	// Verify destination has new content
	QFile verifyFile(destFile);
	verifyFile.open(QIODevice::ReadOnly);
	QString content = QString(verifyFile.readAll());
	verifyFile.close();
	QCOMPARE(content, QString("New move content"));
}


void CSystemTest::EnsurePathExistsTest()
{
	// Test with new path
	QString newPath = m_testDir + "/NewPath/SubPath/DeepPath";
	
	bool success = istd::CSystem::EnsurePathExists(newPath);
	QVERIFY(success);
	QVERIFY(QDir(newPath).exists());
	
	// Test with existing path (should still return true)
	success = istd::CSystem::EnsurePathExists(newPath);
	QVERIFY(success);
}


void CSystemTest::GetCurrentUserNameTest()
{
	QString userName = istd::CSystem::GetCurrentUserName();
	
	// User name may or may not be available depending on OS
	// Just verify the function doesn't crash
	// On Windows and macOS it should return a non-empty string
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
	QVERIFY(!userName.isEmpty());
#else
	// On other platforms, it may be empty
	QVERIFY(true);
#endif
}


void CSystemTest::GetFileDriveInfoTest()
{
	// Test with current directory
	QString testPath = QDir::currentPath();
	istd::CSystem::FileDriveInfo info = istd::CSystem::GetFileDriveInfo(testPath);
	
	// On Qt 5.4+, we should get valid info
#if QT_VERSION >= 0x050400
	QVERIFY(info.isValid);
	QVERIFY(info.totalBytes > 0);
	// Free bytes should be less than or equal to total
	QVERIFY(info.freeBytes <= info.totalBytes);
#else
	// On older Qt versions, behavior depends on platform
	QVERIFY(true);
#endif
}


void CSystemTest::GetCompilerVariableTest()
{
	// Test AcfQtVersion
	QString qtVersion = istd::CSystem::GetCompilerVariable("AcfQtVersion");
	// May be empty or set depending on build configuration
	// If set, it should be a valid Qt version format (e.g., "Qt5" or "Qt6")
	if (!qtVersion.isEmpty())
	{
		QVERIFY(qtVersion.startsWith("Qt") || qtVersion.contains("."));
	}
	
	// Test PlatformCode
	QString platformCode = istd::CSystem::GetCompilerVariable("PlatformCode");
	// May be empty for 32-bit or have value like "x64" for 64-bit
	if (!platformCode.isEmpty())
	{
		QVERIFY(platformCode == "x64" || platformCode.contains("64") || platformCode.contains("32"));
	}
	
	// Test CompileMode
	QString compileMode = istd::CSystem::GetCompilerVariable("CompileMode");
	// Should be either "Debug" or "Release"
	QVERIFY(compileMode == "Debug" || compileMode == "Release");
	
	// Test CompilerName
	QString compilerName = istd::CSystem::GetCompilerVariable("CompilerName");
	QVERIFY(!compilerName.isEmpty());
	
	// Test ConfigurationName
	QString configName = istd::CSystem::GetCompilerVariable("ConfigurationName");
	QVERIFY(!configName.isEmpty());
}


void CSystemTest::SetUserVariablesTest()
{
	// Set custom user variables
	istd::CSystem::SetUserVariables("TestMode", "Qt5", "TestCompiler", "TestPlatform");
	
	// Verify they were set
	QString compileMode = istd::CSystem::GetVariableValue("CompileMode");
	QCOMPARE(compileMode, QString("TestMode"));
	
	QString compilerName = istd::CSystem::GetVariableValue("CompilerName");
	QCOMPARE(compilerName, QString("TestCompiler"));
	
	QString platformCode = istd::CSystem::GetVariableValue("PlatformCode");
	QCOMPARE(platformCode, QString("TestPlatform"));
	
	// Verify ConfigurationName includes all components
	QString configName = istd::CSystem::GetVariableValue("ConfigurationName");
	QVERIFY(configName.contains("TestMode"));
	QVERIFY(configName.contains("Qt5"));
	QVERIFY(configName.contains("TestCompiler"));
	QVERIFY(configName.contains("TestPlatform"));
}


void CSystemTest::ConvertNetworkPathToUncTest()
{
	// Test with local path (should remain unchanged on most platforms)
	QString localPath = "C:/test/path";
	QString uncPath = istd::CSystem::ConvertNetworkPathToUnc(localPath);
	
	// On non-Windows or non-MSVC platforms, should return the same path
	QVERIFY(!uncPath.isEmpty());
	
	// Test with already UNC path
	QString uncInput = "\\\\server\\share\\path";
	QString uncOutput = istd::CSystem::ConvertNetworkPathToUnc(uncInput);
	QVERIFY(!uncOutput.isEmpty());
}


void CSystemTest::GetCompilerInfoTest()
{
	QString compilerInfo = istd::CSystem::GetCompilerInfo();
	
	// Should return non-empty string with compiler information
	QVERIFY(!compilerInfo.isEmpty());
	
	// Should contain "compiler" in the string
	QVERIFY(compilerInfo.toLower().contains("compiler"));
}


void CSystemTest::cleanupTestCase()
{
	// Clean up test directory
	if (!m_testDir.isEmpty() && QDir(m_testDir).exists())
	{
		istd::CSystem::RemoveDirectory(m_testDir);
	}
}


I_ADD_TEST(CSystemTest);
