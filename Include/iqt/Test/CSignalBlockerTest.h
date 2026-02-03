// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iqt/CSignalBlocker.h>
#include <itest/CStandardTestExecutor.h>

class CSignalBlockerTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void TestBlockSingleObject();
	void TestBlockWithChildren();
	void TestNullPointer();
	void TestScopeBasedBlocking();
	void TestNestedBlockers();

	void cleanupTestCase();
};

