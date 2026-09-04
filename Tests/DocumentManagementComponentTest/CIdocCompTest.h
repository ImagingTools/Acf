// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <optional>

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

		// Sets the document change flag expected to be seen while handling the next
		// model update notification. The check is performed directly inside OnUpdate(),
		// so it verifies the state as observed at notification time, not afterwards.
		void SetExpectedDocumentChangeFlag(idoc::IDocumentStateComparator::DocumentChangeFlag expectedFlag)
		{
			m_expectedDocumentChangeFlag = expectedFlag;
		}

	protected:
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/) override
		{
			idoc::IDocumentStateComparator* comparator = dynamic_cast<idoc::IDocumentStateComparator*>(GetObservedModel());
			QVERIFY(comparator != nullptr);

			m_documentChangeFlag = comparator->GetDocumentChangeFlag();

			if (m_expectedDocumentChangeFlag.has_value()){
				QCOMPARE(m_documentChangeFlag, *m_expectedDocumentChangeFlag);
			}
		}

	private:
		idoc::IDocumentStateComparator::DocumentChangeFlag m_documentChangeFlag;
		std::optional<idoc::IDocumentStateComparator::DocumentChangeFlag> m_expectedDocumentChangeFlag;
	};

	std::shared_ptr<CDocumentManagementComponentTest> m_testInstanceCompPtr;

	idoc::ITextDocument* m_textDocumentPtr = nullptr;
	idoc::ITextDocument* m_textDocumentWithDefaultPtr = nullptr;
	idoc::IUndoManager* m_undoManagerPtr = nullptr;
	idoc::IUndoManager* m_undoManagerSmallBufferPtr = nullptr;
	idoc::IDocumentTemplate* m_documentTemplatePtr = nullptr;
};
