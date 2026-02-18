// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CTestInstance.h>


/**
	Forward declaration of test component class.
*/
class CApplicationInfoComponentTest;


/**
	Unit tests for CApplicationInfoComp component.
*/
class CApplicationInfoCompTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	void cleanupTestCase();
	
	// Application Info Component tests
	void testApplicationInfoComponentCreation();
	void testGetApplicationAttribute();
	void testGetCompanyName();
	void testGetProductName();
	void testGetApplicationName();
	void testGetApplicationId();
	void testGetApplicationType();
	void testGetLegalCopyright();
	void testGetMainVersionId();
	void testGetVersionInfo();
	void testCustomValues();

private:
	std::unique_ptr<CApplicationInfoComponentTest> m_testInstanceCompPtr;
	ibase::IApplicationInfo* m_appInfoCompPtr;
	ibase::IApplicationInfo* m_customAppInfoCompPtr;
};


I_ADD_TEST(CApplicationInfoCompTest)
