// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <i2d/CVector2d.h>
#include <itest/CStandardTestExecutor.h>

class CVector2dTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void GetSetTest();
	void ArithmeticOperatorsTest();
	void GetTranslatedTest();
	void GetAngleTest();
	void GetOrthogonalTest();
	void GetNormalizedTest();
	void GetCrossProductZTest();
	void GetDotProductTest();

	void cleanupTestCase();
};


