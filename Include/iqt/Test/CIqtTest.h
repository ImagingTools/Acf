// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iqt/iqt.h>
#include <itest/CStandardTestExecutor.h>

class CIqtTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void TestQSizeConversions();
	void TestQPointConversions();
	void TestQRectFConversions();
	void TestQLineConversions();
	void TestQLineFConversions();
	void TestQRectConversions();

	void cleanupTestCase();
};

