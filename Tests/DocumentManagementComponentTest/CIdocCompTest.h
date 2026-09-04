// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imod/CSingleModelObserverBase.h>
#include <idoc/ITextDocument.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentTemplate.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/DocumentManagementComponentTest/CDocumentManagementComponentTest.h>

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
	void testUndoManagerObserverSeesChangedDocumentState();

	// Tests for CSingleDocumentTemplateComp
	void testDocumentTemplateCreation();
	void testDocumentTemplateCreateDocument();
	void testDocumentTemplateAttributes();

	void cleanupTestCase();

private:
	class CDocumentStateObserver: public imod::CSingleModelObserverBase
	{
	public:
		CDocumentStateObserver()
			: m_documentChangeFlag(idoc::IDocumentStateComparator::DCF_UNKNOWN)
		{
		}

		idoc::IDocumentStateComparator::DocumentChangeFlag GetDocumentChangeFlag() const
		{
			return m_documentChangeFlag;
		}

	protected:
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/) override
		{
			idoc::IDocumentStateComparator* comparator = dynamic_cast<idoc::IDocumentStateComparator*>(GetObservedModel());
			if (comparator != nullptr){
				m_documentChangeFlag = comparator->GetDocumentChangeFlag();
			}
		}

	private:
		idoc::IDocumentStateComparator::DocumentChangeFlag m_documentChangeFlag;
	};

	std::shared_ptr<CDocumentManagementComponentTest> m_testInstanceCompPtr;

	idoc::ITextDocument* m_textDocumentPtr = nullptr;
	idoc::ITextDocument* m_textDocumentWithDefaultPtr = nullptr;
	idoc::IUndoManager* m_undoManagerPtr = nullptr;
	idoc::IUndoManager* m_undoManagerSmallBufferPtr = nullptr;
	idoc::IDocumentTemplate* m_documentTemplatePtr = nullptr;
};
