// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/IEnableableParam.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/EnableableParamComponentTest/CEnableableParamComponentTest.h>

class CEnableableParamCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for enableable parameter with default enabled
	void testCreationDefaultEnabled();
	void testGetEnabledState();
	void testSetEnabled();
	void testToggleEnabled();
	void testSerializationEnabled();
	void testCopyEnabled();
	void testCloneEnabled();

	// Tests for enableable parameter with default disabled
	void testCreationDefaultDisabled();
	void testDisabledState();
	void testSerializationDisabled();

	void cleanupTestCase();

private:
	std::shared_ptr<CEnableableParamComponentTest> m_testPartituraInstanceCompPtr;

	iprm::IEnableableParam* m_enabledByDefaultPtr = nullptr;
	iprm::IEnableableParam* m_disabledByDefaultPtr = nullptr;
};
