// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


/**
	Unit tests for TMsbWord template class.
*/
class TMsbWordTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	
	// Constructor tests
	void testDefaultConstructor();
	void testValueConstructor();
	void testCopyConstructor();
	
	// Conversion tests
	void testMsbConversion();
	void testLsbConversion();
	void testLsbMsbRoundtrip();
	
	// Different sizes
	void testByteSize();
	void testWordSize();
	void testDWordSize();
	
	// Serialization tests
	void testSerialization();
	
	void cleanupTestCase();
};


I_ADD_TEST(TMsbWordTest)
