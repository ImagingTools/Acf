// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


/**
	Unit tests for TContainer template class.
*/
class TContainerTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	
	// Basic operations
	void testPushBack();
	void testPushFront();
	void testInsertAt();
	void testGetAt();
	
	// Removal operations
	void testPopBack();
	void testPopFront();
	void testRemoveAt();
	void testReset();
	
	// Container info
	void testGetItemsCount();
	void testIsEmpty();
	void testIsIndexValid();
	
	// Operators
	void testSubscriptOperator();
	void testAssignmentOperator();
	void testEqualityOperators();
	
	// Iterators
	void testIterators();
	
	// Standard container interface
	void testStandardPushOperations();
	void testStandardInsert();
	void testStandardRemove();
	void testStandardResize();
	void testStandardSize();
	
	void cleanupTestCase();
};


