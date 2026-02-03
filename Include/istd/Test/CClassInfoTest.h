// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CClassInfo.h>
#include <itest/CStandardTestExecutor.h>

class CClassInfoTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void TypeInfoConstructorTest();
	void NameConstructorTest();
	void GetNameTest();
	void IsValidTest();
	void ComparisonOperatorsTest();
	void IsTypeTest();
	void GetInfoStaticTest();

	void cleanupTestCase();
};


