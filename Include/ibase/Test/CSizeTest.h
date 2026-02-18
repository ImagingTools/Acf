// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


/**
	Unit tests for CSize class.
*/
class CSizeTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	
	// Constructor tests
	void testDefaultConstructor();
	void testParameterizedConstructor();
	void testIndex2dConstructor();
	void testQPointConstructor();
	void testQSizeConstructor();
	
	// Operator tests
	void testAdditionOperator();
	void testSubtractionOperator();
	void testPlusEqualsOperator();
	void testMinusEqualsOperator();
	
	// Method tests
	void testIsNull();
	void testSerialization();
	
	void cleanupTestCase();
};


I_ADD_TEST(CSizeTest)
