// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/TIndex.h>
#include <itest/CStandardTestExecutor.h>

class TIndexTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void ValueConstructorTest();
	void CopyConstructorTest();
	void IsValidTest();
	void IsZeroTest();
	void IsSizeEmptyTest();
	void GetSetAtTest();
	void ArrayAccessTest();
	void ComparisonOperatorsTest();
	void ArithmeticOperatorsTest();

	void cleanupTestCase();
};


