// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtTest/QtTest>

// ACF includes
#include <istd/TNullableValue.h>
#include <itest/CStandardTestExecutor.h>

class TNullableValueTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void ValidityMethodsTest();
	void PointerAccessTest();
	void ValueAccessTest();
	void ResetMethodsTest();
	void EmplaceMethodsTest();
	void SetNullAndValueOrTest();
	void AssignmentOperatorsTest();
	void BoolAndHasValueTest();
	void ComparisonOperatorsTest();
	void SwapAndLessTest();

	void cleanupTestCase();
};

