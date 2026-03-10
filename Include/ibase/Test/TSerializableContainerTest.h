// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


/**
	Unit tests for TSerializableContainer template class.
*/
class TSerializableContainerTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	
	// Serialization tests
	void testSerializeInt();
	void testSerializeString();
	void testSerializeEmpty();
	
	void cleanupTestCase();
};


