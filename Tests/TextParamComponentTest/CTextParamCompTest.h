// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/ITextParam.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/TextParamComponentTest/CTextParamComponentTest.h>

class CTextParamCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for text parameter with default text
	void testCreationWithDefault();
	void testGetDefaultText();
	void testSetText();
	void testSerializationWithDefault();
	void testCopyWithDefault();
	void testCloneWithDefault();

	// Tests for text parameter without default
	void testCreationNoDefault();
	void testEmptyText();
	void testSerializationNoDefault();

	// Tests for read-only text parameter
	void testReadOnlyCreation();
	void testReadOnlyBehavior();
	void testReadOnlySerialization();

	// Tests for serialization control
	void testNoSerializationParam();

	void cleanupTestCase();

private:
	std::shared_ptr<CTextParamComponentTest> m_testInstancePtr;

	iprm::ITextParam* m_textWithDefaultPtr = nullptr;
	iprm::ITextParam* m_textNoDefaultPtr = nullptr;
	iprm::ITextParam* m_textReadOnlyPtr = nullptr;
	iprm::ITextParam* m_textNoSerializePtr = nullptr;
};
