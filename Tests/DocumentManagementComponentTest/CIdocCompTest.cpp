// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CIdocCompTest.h"


// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <idoc/IDocumentStateComparator.h>


// protected slots

void CIdocCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CDocumentManagementComponentTest);

	m_textDocumentPtr = m_testInstanceCompPtr->GetInterface<idoc::ITextDocument>("TextDocument");
	m_textDocumentWithDefaultPtr = m_testInstanceCompPtr->GetInterface<idoc::ITextDocument>("TextDocumentWithDefault");
	m_undoManagerPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManager");
	m_undoManagerSmallBufferPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManagerSmallBuffer");
	m_documentTemplatePtr = m_testInstanceCompPtr->GetInterface<idoc::IDocumentTemplate>("DocumentTemplate");

	QVERIFY(m_textDocumentPtr != nullptr);
	QVERIFY(m_textDocumentWithDefaultPtr != nullptr);
	QVERIFY(m_undoManagerPtr != nullptr);
	QVERIFY(m_undoManagerSmallBufferPtr != nullptr);
	QVERIFY(m_documentTemplatePtr != nullptr);
}


// Tests for CTextDocumentComp

void CIdocCompTest::testTextDocumentCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_textDocumentPtr != nullptr);
	
	// Check that the text is not null
	QString text = m_textDocumentPtr->GetText();
	QVERIFY(!text.isNull());
}


void CIdocCompTest::testTextDocumentGetSet()
{
	// Set a specific text
	QString testText = "Test document content";
	m_textDocumentPtr->SetText(testText);
	
	// Verify the text was set correctly
	QCOMPARE(m_textDocumentPtr->GetText(), testText);
	
	// Set different text
	QString newText = "Updated content";
	m_textDocumentPtr->SetText(newText);
	QCOMPARE(m_textDocumentPtr->GetText(), newText);
	
	// Test with empty string
	m_textDocumentPtr->SetText("");
	QCOMPARE(m_textDocumentPtr->GetText(), QString(""));
}


void CIdocCompTest::testTextDocumentDefaultText()
{
	// Verify the document with default text has the configured value
	QString defaultText = m_textDocumentWithDefaultPtr->GetText();
	QCOMPARE(defaultText, QString("Custom Default Text"));
}


void CIdocCompTest::testTextDocumentSerialization()
{
	// Set a specific text
	QString testText = "Serialization test content";
	m_textDocumentPtr->SetText(testText);
	
	// Serialize to memory
	iser::ISerializable* serializable = dynamic_cast<iser::ISerializable*>(m_textDocumentPtr);
	QVERIFY(serializable != nullptr);
	
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(serializable->Serialize(writeArchive));
	
	// Change the text
	m_textDocumentPtr->SetText("Different text");
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Different text"));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(serializable->Serialize(readArchive));
	
	// Verify the text was restored
	QCOMPARE(m_textDocumentPtr->GetText(), testText);
}


// Tests for CSerializedUndoManagerComp

void CIdocCompTest::testUndoManagerCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_undoManagerPtr != nullptr);
	
	// Check initial state - should have no undo/redo steps
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CIdocCompTest::testUndoManagerUndoRedo()
{
	// Get the document as IChangeable for undo manager attachment
	istd::IChangeable* changeable = dynamic_cast<istd::IChangeable*>(m_textDocumentPtr);
	QVERIFY(changeable != nullptr);
	
	// Get undo manager as observer
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);
	
	// Attach undo manager to document
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	observer->AttachModel(model);
	
	// Set initial text
	m_textDocumentPtr->SetText("Initial text");
	
	// Make a change - this should create an undo step
	m_textDocumentPtr->SetText("Modified text");
	
	// Verify we have an undo step
	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
	
	// Perform undo
	QVERIFY(m_undoManagerPtr->DoUndo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Initial text"));
	
	// After undo, we should have a redo step
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() > 0);
	
	// Perform redo
	QVERIFY(m_undoManagerPtr->DoRedo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Modified text"));
	
	// Clean up
	observer->DetachModel(model);
	m_undoManagerPtr->ResetUndo();
}


void CIdocCompTest::testUndoManagerMultipleSteps()
{
	// Get the document as IChangeable
	istd::IChangeable* changeable = dynamic_cast<istd::IChangeable*>(m_textDocumentPtr);
	QVERIFY(changeable != nullptr);
	
	// Get undo manager as observer
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);
	
	// Attach undo manager to document
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	observer->AttachModel(model);
	
	// Reset undo to start fresh
	m_undoManagerPtr->ResetUndo();
	
	// Make multiple changes
	m_textDocumentPtr->SetText("Step 0");
	m_textDocumentPtr->SetText("Step 1");
	m_textDocumentPtr->SetText("Step 2");
	m_textDocumentPtr->SetText("Step 3");
	
	// Should have 3 undo steps (changes from step 0 to 1, 1 to 2, 2 to 3)
	int undoSteps = m_undoManagerPtr->GetAvailableUndoSteps();
	QVERIFY(undoSteps >= 3);
	
	// Undo twice
	QVERIFY(m_undoManagerPtr->DoUndo(2));
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Step 1"));
	
	// Should have 2 redo steps available
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() >= 2);
	
	// Clean up
	observer->DetachModel(model);
	m_undoManagerPtr->ResetUndo();
}


void CIdocCompTest::testUndoManagerMaxBufferSize()
{
	// Verify that the small buffer undo manager was created
	QVERIFY(m_undoManagerSmallBufferPtr != nullptr);
	
	// The small buffer is configured with MaxBufferSize=1 (1 MB)
	// This test verifies the component creation with different buffer size
	QCOMPARE(m_undoManagerSmallBufferPtr->GetAvailableUndoSteps(), 0);
}


void CIdocCompTest::testUndoManagerReset()
{
	// Get the document as IChangeable
	istd::IChangeable* changeable = dynamic_cast<istd::IChangeable*>(m_textDocumentPtr);
	QVERIFY(changeable != nullptr);
	
	// Get undo manager as observer
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);
	
	// Attach undo manager to document
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	observer->AttachModel(model);
	
	// Reset to start fresh
	m_undoManagerPtr->ResetUndo();
	
	// Make some changes
	m_textDocumentPtr->SetText("Change 1");
	m_textDocumentPtr->SetText("Change 2");
	
	// Verify we have undo steps
	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);
	
	// Reset undo
	m_undoManagerPtr->ResetUndo();
	
	// Verify all undo/redo steps are cleared
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
	
	// Clean up
	observer->DetachModel(model);
}


void CIdocCompTest::testUndoManagerStateComparison()
{
	// Get state comparator interface
	idoc::IDocumentStateComparator* comparator = 
		dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr);
	QVERIFY(comparator != nullptr);
	
	// Initially should not have stored state
	QVERIFY(!comparator->HasStoredDocumentState());
	
	// Store document state
	QVERIFY(comparator->StoreDocumentState());
	QVERIFY(comparator->HasStoredDocumentState());
	
	// Restore document state
	QVERIFY(comparator->RestoreDocumentState());
	
	// After restoration, stored state should still exist
	QVERIFY(comparator->HasStoredDocumentState());
}


// Tests for CSingleDocumentTemplateComp

void CIdocCompTest::testDocumentTemplateCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_documentTemplatePtr != nullptr);
	
	// Check that template provides basic information
	QVERIFY(m_documentTemplatePtr->GetDocumentTypeId().size() > 0);
}


void CIdocCompTest::testDocumentTemplateCreateDocument()
{
	// Get the document type ID
	QByteArray docTypeId = m_documentTemplatePtr->GetDocumentTypeId();
	QVERIFY(!docTypeId.isEmpty());
	
	// Create a new document using the template
	istd::IChangeableUniquePtr documentPtr = m_documentTemplatePtr->CreateDocument(docTypeId);
	QVERIFY(documentPtr.get() != nullptr);
	
	// Verify the document implements ITextDocument
	idoc::ITextDocument* textDoc = dynamic_cast<idoc::ITextDocument*>(documentPtr.get());
	QVERIFY(textDoc != nullptr);
	
	// Verify the document has the default text
	QCOMPARE(textDoc->GetText(), QString("Template Default Text"));
}


void CIdocCompTest::testDocumentTemplateAttributes()
{
	// Verify document type ID
	QByteArray docTypeId = m_documentTemplatePtr->GetDocumentTypeId();
	QCOMPARE(docTypeId, QByteArray("TestDocType"));
	
	// Check if new operation is supported
	QVERIFY(m_documentTemplatePtr->IsNewSupported(docTypeId));
	
	// Check if edit operation is supported
	QVERIFY(m_documentTemplatePtr->IsEditSupported(docTypeId));
	
	// Get document type name
	QString docTypeName = m_documentTemplatePtr->GetDocumentTypeName(docTypeId);
	QCOMPARE(docTypeName, QString("Test Document Type"));
}


void CIdocCompTest::cleanupTestCase()
{
	m_testInstanceCompPtr.reset();
}


I_ADD_TEST(CIdocCompTest);
