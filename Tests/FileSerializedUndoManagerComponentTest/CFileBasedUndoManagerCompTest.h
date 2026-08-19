// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtTest/QtTest>

// ACF includes
#include <iser/ISerializable.h>
#include <idoc/ITextDocument.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentStateComparator.h>
#include <ifile/IFileNameParam.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/FileSerializedUndoManagerComponentTest/CFileSerializedUndoManagerComponentTest.h>

/**
	Component tests dedicated to idoc::CFileBasedUndoManagerComp (file-based serialized undo manager).

	The undo step files are stored inside a unique directory created below the system temporary
	directory. This directory is removed again in \ref cleanupTestCase, so that no test artifacts
	are left behind on the file system.
*/
class CFileBasedUndoManagerCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void init();
	void cleanup();

	void testCreation();
	void testStorageDirectoryInTemp();
	void testUndoRedoSingleStep();
	void testMultipleSteps();
	void testUndoHistoryIsNotTrimmedByBufferSize();
	void testReset();
	void testSerializeRestoresCurrentStep();

	void cleanupTestCase();

private:
	std::shared_ptr<CFileSerializedUndoManagerComponentTest> m_testInstanceCompPtr;

	idoc::ITextDocument* m_textDocumentPtr = nullptr;
	idoc::IUndoManager* m_undoManagerPtr = nullptr;
	idoc::IUndoManager* m_undoManagerRestorePtr = nullptr;
	ifile::IFileNameParam* m_storageDirectoryPtr = nullptr;
	ifile::IFileNameParam* m_storageDirectoryRestorePtr = nullptr;

	QString m_tempRootPath;
};
