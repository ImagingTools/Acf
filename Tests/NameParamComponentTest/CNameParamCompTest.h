// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/INameParam.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/NameParamComponentTest/CNameParamComponentTest.h>

class CNameParamCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for name parameter with default name
	void testCreationWithDefault();
	void testGetDefaultName();
	void testSetName();
	void testSerializationWithDefault();
	void testCopyWithDefault();
	void testCloneWithDefault();

	// Tests for name parameter without default
	void testCreationNoDefault();
	void testEmptyName();

	// Tests for fixed name parameter
	void testFixedNameCreation();
	void testFixedNameBehavior();

	void cleanupTestCase();

private:
	std::shared_ptr<CNameParamComponentTest> m_testInstancePtr;

	iprm::INameParam* m_nameWithDefaultPtr = nullptr;
	iprm::INameParam* m_nameNoDefaultPtr = nullptr;
	iprm::INameParam* m_nameFixedPtr = nullptr;
};
