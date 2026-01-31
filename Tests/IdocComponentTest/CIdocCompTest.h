// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <idoc/ITextDocument.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentTemplate.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/IdocComponentTest/CIdocComponentTest.h>

class CIdocCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for CTextDocumentComp
	void testTextDocumentCreation();
	void testTextDocumentGetSet();
	void testTextDocumentDefaultText();
	void testTextDocumentSerialization();

	// Tests for CSerializedUndoManagerComp
	void testUndoManagerCreation();
	void testUndoManagerUndoRedo();
	void testUndoManagerMultipleSteps();
	void testUndoManagerMaxBufferSize();
	void testUndoManagerReset();
	void testUndoManagerStateComparison();

	// Tests for CSingleDocumentTemplateComp
	void testDocumentTemplateCreation();
	void testDocumentTemplateCreateDocument();
	void testDocumentTemplateAttributes();

	void cleanupTestCase();

private:
	std::shared_ptr<CIdocComponentTest> m_testInstanceCompPtr;

	idoc::ITextDocument* m_textDocumentPtr = nullptr;
	idoc::ITextDocument* m_textDocumentWithDefaultPtr = nullptr;
	idoc::IUndoManager* m_undoManagerPtr = nullptr;
	idoc::IUndoManager* m_undoManagerSmallBufferPtr = nullptr;
	idoc::IDocumentTemplate* m_documentTemplatePtr = nullptr;
};
