// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/IIdParam.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/IdParamComponentTest/CIdParamComponentTest.h>

class CIdParamCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for ID parameter with default ID
	void testCreationWithDefault();
	void testGetDefaultId();
	void testSetId();
	void testBinaryId();
	void testSerializationWithDefault();
	void testCopyWithDefault();
	void testCloneWithDefault();

	// Tests for ID parameter without default
	void testCreationNoDefault();
	void testEmptyId();
	void testSerializationNoDefault();

	void cleanupTestCase();

private:
	std::shared_ptr<CIdParamComponentTest> m_testInstancePtr;

	iprm::IIdParam* m_idWithDefaultPtr = nullptr;
	iprm::IIdParam* m_idNoDefaultPtr = nullptr;
};
