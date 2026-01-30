// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsManager.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/SelectionParamComponentTest/CSelectionParamComponentTest.h>

class CSelectionParamCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for selection parameter without constraints
	void testCreationNoConstraints();
	void testDefaultIndexNoConstraints();
	void testSerializationNoConstraints();
	void testCopyNoConstraints();
	void testCloneNoConstraints();

	// Tests for selection parameter with constraints
	void testCreationWithConstraints();
	void testDefaultIndexWithConstraints();
	void testInvalidDefaultIndexWithConstraints();
	void testSerializationWithConstraints();
	void testCopyWithConstraints();
	void testCloneWithConstraints();

	// Tests for selection parameter with subselection
	void testCreationWithSubselection();
	void testGetSubselection();
	void testSerializationWithSubselection();

	void cleanupTestCase();

private:
	std::shared_ptr<CSelectionParamComponentTest> m_testPartituraInstanceCompPtr;

	iprm::ISelectionParam* m_selectionNoConstraintsPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithDefaultIndexPtr = nullptr;
	iprm::IOptionsManager* m_optionsConstraintsPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithConstraintsPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithConstraintsAndDefaultPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithInvalidDefaultPtr = nullptr;
	iprm::ISelectionParam* m_subselectionPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithSubselectionPtr = nullptr;
};
