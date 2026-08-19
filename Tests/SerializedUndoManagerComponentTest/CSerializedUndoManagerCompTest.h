// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <idoc/ITextDocument.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentStateComparator.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/SerializedUndoManagerComponentTest/CSerializedUndoManagerComponentTest.h>

/**
	Component tests dedicated to idoc::CSerializedUndoManagerComp (in-memory serialized undo manager).
*/
class CSerializedUndoManagerCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void init();
	void cleanup();

	void testCreation();
	void testUndoRedoSingleStep();
	void testMultipleSteps();
	void testRedoClearedAfterNewChange();
	void testReset();
	void testMaxBufferSizeLimit();
	void testDocumentStateComparator();

	void cleanupTestCase();

private:
	std::shared_ptr<CSerializedUndoManagerComponentTest> m_testInstanceCompPtr;

	idoc::ITextDocument* m_textDocumentPtr = nullptr;
	idoc::IUndoManager* m_undoManagerPtr = nullptr;
	idoc::IUndoManager* m_undoManagerSmallBufferPtr = nullptr;
};
