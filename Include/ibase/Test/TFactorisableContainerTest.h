// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <ibase/TFactorisableContainer.h>


/**
	Unit tests for TFactorisableContainer template class.
*/
class TFactorisableContainerTest : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	
	// Element management tests
	void testAddElement();
	void testInsertElement();
	void testGetElement();
	void testGetElementIndex();
	void testGetElementKey();
	
	// Serialization tests
	void testSerialize();
	void testSerializeEmpty();
	
	void cleanupTestCase();
};


