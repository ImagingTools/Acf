// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CSerializedUndoManagerCompTest.h"


// ACF includes
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>


// protected slots

void CSerializedUndoManagerCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CSerializedUndoManagerComponentTest);

	m_textDocumentPtr = m_testInstanceCompPtr->GetInterface<idoc::ITextDocument>("TextDocument");
	m_undoManagerPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManager");
	m_undoManagerSmallBufferPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManagerSmallBuffer");

	QVERIFY(m_textDocumentPtr != nullptr);
	QVERIFY(m_undoManagerPtr != nullptr);
	QVERIFY(m_undoManagerSmallBufferPtr != nullptr);
}


void CSerializedUndoManagerCompTest::init()
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


void CSerializedUndoManagerCompTest::cleanup()
{
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	if ((model != nullptr) && (observer != nullptr)){
		m_undoManagerPtr->ResetUndo();
		model->DetachObserver(observer);
	}
}


void CSerializedUndoManagerCompTest::testCreation()
{
	// The component exposes all expected interfaces.
	QVERIFY(dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr) != nullptr);
	QVERIFY(dynamic_cast<imod::IObserver*>(m_undoManagerPtr) != nullptr);

	// A freshly reset undo manager has no undo/redo steps.
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CSerializedUndoManagerCompTest::testUndoRedoSingleStep()
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


void CSerializedUndoManagerCompTest::testMultipleSteps()
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


void CSerializedUndoManagerCompTest::testRedoClearedAfterNewChange()
{
	m_textDocumentPtr->SetText("A");
	m_textDocumentPtr->SetText("B");

	QVERIFY(m_undoManagerPtr->DoUndo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("A"));
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() > 0);

	// A new change after an undo must invalidate the redo history.
	m_textDocumentPtr->SetText("C");
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CSerializedUndoManagerCompTest::testReset()
{
	m_textDocumentPtr->SetText("Change 1");
	m_textDocumentPtr->SetText("Change 2");
	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);

	m_undoManagerPtr->ResetUndo();
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CSerializedUndoManagerCompTest::testMaxBufferSizeLimit()
{
	// The small-buffer manager is configured with MaxBufferSize = 1 MByte.
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	imod::IObserver* smallObserver = dynamic_cast<imod::IObserver*>(m_undoManagerSmallBufferPtr);
	QVERIFY(model != nullptr);
	QVERIFY(smallObserver != nullptr);

	model->AttachObserver(smallObserver);
	m_undoManagerSmallBufferPtr->ResetUndo();

	// Each state stores about 0.7 MByte, so the undo buffer cannot keep all of them.
	const int changeCount = 6;
	for (int i = 0; i < changeCount; ++i){
		m_textDocumentPtr->SetText(QString::number(i) + QString(700 * 1024, QChar('a')));
	}

	int undoSteps = m_undoManagerSmallBufferPtr->GetAvailableUndoSteps();
	QVERIFY(undoSteps >= 1);
	QVERIFY(undoSteps < changeCount);

	m_undoManagerSmallBufferPtr->ResetUndo();
	model->DetachObserver(smallObserver);
}


void CSerializedUndoManagerCompTest::testDocumentStateComparator()
{
	idoc::IDocumentStateComparator* comparator =
		dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr);
	QVERIFY(comparator != nullptr);

	m_textDocumentPtr->SetText("Stored state");
	QVERIFY(comparator->StoreDocumentState());
	QVERIFY(comparator->HasStoredDocumentState());
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);

	// After a change the current state differs from the stored one.
	m_textDocumentPtr->SetText("Changed state");
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_DIFFERENT);

	// Restoring brings back the stored content.
	QVERIFY(comparator->RestoreDocumentState());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Stored state"));
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);
}


void CSerializedUndoManagerCompTest::cleanupTestCase()
{
	m_testInstanceCompPtr.reset();
}


I_ADD_TEST(CSerializedUndoManagerCompTest);
