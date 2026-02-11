// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


/**
	Unit tests for CObservableListBase class.
*/
class CObservableListBaseTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	
	// ChangeSet generation tests
	void testElementAddChanges();
	void testElementRemoveChanges();
	void testElementUpdatedChanges();
	void testChangeFlagValues();
	void testIndexInChangeSet();
	
	void cleanupTestCase();
};


I_ADD_TEST(CObservableListBaseTest)
