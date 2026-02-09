// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imath/CVarVector.h>
#include <itest/CStandardTestExecutor.h>

class CVarVectorTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void SizeConstructorTest();
	void CopyConstructorTest();
	void TVectorConstructorTest();
	void IsEmptyTest();
	void GetSetElementTest();
	void SetAllElementsTest();
	void ResetClearTest();
	void SetElementsFromTest();
	void EnsureElementsCountTest();
	void TranslateTest();
	void ScaledCumulateTest();
	void IsNullTest();
	void DotProductTest();
	void LengthTest();
	void DistanceTest();
	void ElementsSumTest();
	void NormalizeTest();
	void GetNormalizedTest();

	void cleanupTestCase();
};


