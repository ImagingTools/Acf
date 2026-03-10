// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/TNullable.h>
#include <itest/CStandardTestExecutor.h>

class TNullableTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void ValueConstructorTest();
	void IsValidTest();
	void IsNullTest();
	void HasValueTest();
	void SetNullTest();
	void ResetTest();
	void EmplaceTest();
	void GetValueTest();
	void ValueOrTest();
	void ComparisonOperatorsTest();
	void AssignmentOperatorTest();

	void cleanupTestCase();
};


