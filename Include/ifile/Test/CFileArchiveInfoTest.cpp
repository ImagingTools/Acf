// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CFileArchiveInfoTest.h>


// ACF includes
#include <ifile/CFileArchiveInfo.h>


void CFileArchiveInfoTest::DoFilePathTest()
{
	QString testPath = "./test/path/file.xml";
	ifile::CFileArchiveInfo archiveInfo(testPath);
	
	QCOMPARE(archiveInfo.GetCurrentFilePath(), testPath);
}


void CFileArchiveInfoTest::DoEmptyPathTest()
{
	QString emptyPath = "";
	ifile::CFileArchiveInfo archiveInfo(emptyPath);
	
	QCOMPARE(archiveInfo.GetCurrentFilePath(), emptyPath);
}


void CFileArchiveInfoTest::DoRelativePathTest()
{
	QString relativePath = "../data/test.xml";
	ifile::CFileArchiveInfo archiveInfo(relativePath);
	
	QCOMPARE(archiveInfo.GetCurrentFilePath(), relativePath);
}


void CFileArchiveInfoTest::DoAbsolutePathTest()
{
	QString absolutePath = "/home/user/documents/file.xml";
	ifile::CFileArchiveInfo archiveInfo(absolutePath);
	
	QCOMPARE(archiveInfo.GetCurrentFilePath(), absolutePath);
}


I_ADD_TEST(CFileArchiveInfoTest);


