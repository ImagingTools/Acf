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
class CUuidComponentTest;


/**
	Unit tests for CUuidComp component.
*/
class CUuidCompTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	void cleanupTestCase();
	
	// UUID Component tests
	void testUuidComponentCreation();
	void testUuidGeneration();
	void testUuidUniqueness();
	void testUuidFormat();
	void testUuidSerialization();

private:
	std::unique_ptr<CUuidComponentTest> m_testInstanceCompPtr;
	iprm::INameParam* m_uuidCompPtr1;
	iprm::INameParam* m_uuidCompPtr2;
	iprm::INameParam* m_uuidCompPtr3;
};


I_ADD_TEST(CUuidCompTest)
