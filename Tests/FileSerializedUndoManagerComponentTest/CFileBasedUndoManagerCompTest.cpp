// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CFileBasedUndoManagerCompTest.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

// ACF includes
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <iser/CMemoryWriteArchive.h>
#include <iser/CMemoryReadArchive.h>


// protected slots

void CFileBasedUndoManagerCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CFileSerializedUndoManagerComponentTest);

	m_textDocumentPtr = m_testInstanceCompPtr->GetInterface<idoc::ITextDocument>("TextDocument");
	m_undoManagerPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManager");
	m_undoManagerRestorePtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManagerRestore");
	m_storageDirectoryPtr = m_testInstanceCompPtr->GetInterface<ifile::IFileNameParam>("StorageDirectory");
	m_storageDirectoryRestorePtr = m_testInstanceCompPtr->GetInterface<ifile::IFileNameParam>("StorageDirectoryRestore");

	QVERIFY(m_textDocumentPtr != nullptr);
	QVERIFY(m_undoManagerPtr != nullptr);
	QVERIFY(m_undoManagerRestorePtr != nullptr);
	QVERIFY(m_storageDirectoryPtr != nullptr);
	QVERIFY(m_storageDirectoryRestorePtr != nullptr);

	// Create a unique storage location below the system temporary directory. Each undo manager
	// uses a separate sub-directory so that the round-trip test can keep the stored files while
	// a second manager restores its state.
	QString uniqueName = QString("AcfFileUndoManagerTest_%1").arg(QCoreApplication::applicationPid());
	m_tempRootPath = QDir(QDir::tempPath()).filePath(uniqueName);

	QDir tempRoot(m_tempRootPath);
	QVERIFY(tempRoot.mkpath("steps"));
	QVERIFY(tempRoot.mkpath("restore"));

	m_storageDirectoryPtr->SetPath(tempRoot.filePath("steps"));
	m_storageDirectoryRestorePtr->SetPath(tempRoot.filePath("restore"));
}


void CFileBasedUndoManagerCompTest::init()
{
	// Start every test from a clean, attached state.
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(model != nullptr);
	QVERIFY(observer != nullptr);

	m_textDocumentPtr->SetText("");
	model->AttachObserver(observer);
	m_undoManagerPtr->ResetUndo();
}


void CFileBasedUndoManagerCompTest::cleanup()
{
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	if ((model != nullptr) && (observer != nullptr)){
		m_undoManagerPtr->ResetUndo();
		model->DetachObserver(observer);
	}
}


void CFileBasedUndoManagerCompTest::testCreation()
{
	// The component exposes all expected interfaces.
	QVERIFY(dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr) != nullptr);
	QVERIFY(dynamic_cast<imod::IObserver*>(m_undoManagerPtr) != nullptr);
	QVERIFY(dynamic_cast<iser::ISerializable*>(m_undoManagerPtr) != nullptr);

	// A freshly reset undo manager has no undo/redo steps.
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CFileBasedUndoManagerCompTest::testStorageDirectoryInTemp()
{
	// The configured storage directory has to live below the system temporary directory.
	QString canonicalTemp = QDir(QDir::tempPath()).canonicalPath();
	QString storagePath = m_storageDirectoryPtr->GetPath();
	QVERIFY(!storagePath.isEmpty());
	QVERIFY(storagePath.startsWith(canonicalTemp) || storagePath.startsWith(QDir::tempPath()));

	// Producing undo steps has to create backing files inside that directory.
	m_textDocumentPtr->SetText("First");
	m_textDocumentPtr->SetText("Second");

	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);

	QDir storageDir(storagePath);
	QVERIFY(storageDir.exists());
	QVERIFY(!storageDir.entryList(QStringList() << "*.bin", QDir::Files).isEmpty());
}


void CFileBasedUndoManagerCompTest::testUndoRedoSingleStep()
{
	m_textDocumentPtr->SetText("Initial text");
	m_textDocumentPtr->SetText("Modified text");

	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);

	// Undo restores the previous content and produces a redo step.
	QVERIFY(m_undoManagerPtr->DoUndo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Initial text"));
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() > 0);

	// Redo re-applies the change.
	QVERIFY(m_undoManagerPtr->DoRedo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Modified text"));
}


void CFileBasedUndoManagerCompTest::testMultipleSteps()
{
	m_textDocumentPtr->SetText("Step 0");
	m_textDocumentPtr->SetText("Step 1");
	m_textDocumentPtr->SetText("Step 2");
	m_textDocumentPtr->SetText("Step 3");

	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() >= 3);

	// Undo two steps at once.
	QVERIFY(m_undoManagerPtr->DoUndo(2));
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Step 1"));
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() >= 2);

	// Redo two steps at once.
	QVERIFY(m_undoManagerPtr->DoRedo(2));
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Step 3"));
}


void CFileBasedUndoManagerCompTest::testReset()
{
	m_textDocumentPtr->SetText("Change 1");
	m_textDocumentPtr->SetText("Change 2");
	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);

	m_undoManagerPtr->ResetUndo();
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CFileBasedUndoManagerCompTest::testSerializeRestoresCurrentStep()
{
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	imod::IObserver* restoreObserver = dynamic_cast<imod::IObserver*>(m_undoManagerRestorePtr);
	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(m_undoManagerPtr);
	iser::ISerializable* restoreSerializablePtr = dynamic_cast<iser::ISerializable*>(m_undoManagerRestorePtr);
	QVERIFY(model != nullptr);
	QVERIFY(observer != nullptr);
	QVERIFY(restoreObserver != nullptr);
	QVERIFY(serializablePtr != nullptr);
	QVERIFY(restoreSerializablePtr != nullptr);

	// Build an undo history and move the current position back by one step.
	m_textDocumentPtr->SetText("State A");
	m_textDocumentPtr->SetText("State B");
	m_textDocumentPtr->SetText("State C");

	QVERIFY(m_undoManagerPtr->DoUndo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("State B"));

	int expectedUndoSteps = m_undoManagerPtr->GetAvailableUndoSteps();
	int expectedRedoSteps = m_undoManagerPtr->GetAvailableRedoSteps();
	QString expectedCurrentText = m_textDocumentPtr->GetText();

	// Persist the undo manager, including the observed document state at the current step.
	iser::CMemoryWriteArchive writeArchive;
	QVERIFY(serializablePtr->Serialize(writeArchive));

	// Detach the first manager and bring the document into a different state.
	model->DetachObserver(observer);

	m_undoManagerRestorePtr->ResetUndo();
	model->AttachObserver(restoreObserver);
	m_textDocumentPtr->SetText("Unrelated content");

	// Reading the undo manager must rebuild the history and restore the observed document to the
	// content it had at the current step.
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(restoreSerializablePtr->Serialize(readArchive));

	QCOMPARE(m_undoManagerRestorePtr->GetAvailableUndoSteps(), expectedUndoSteps);
	QCOMPARE(m_undoManagerRestorePtr->GetAvailableRedoSteps(), expectedRedoSteps);
	QCOMPARE(m_textDocumentPtr->GetText(), expectedCurrentText);

	// The restored history stays functional: a redo re-applies the last undone change.
	QVERIFY(m_undoManagerRestorePtr->DoRedo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("State C"));

	m_undoManagerRestorePtr->ResetUndo();
	model->DetachObserver(restoreObserver);
}


void CFileBasedUndoManagerCompTest::cleanupTestCase()
{
	// Remove all step files created during the tests below the temporary directory.
	if (!m_tempRootPath.isEmpty()){
		QDir(m_tempRootPath).removeRecursively();
	}

	m_testInstanceCompPtr.reset();
}


I_ADD_TEST(CFileBasedUndoManagerCompTest);
