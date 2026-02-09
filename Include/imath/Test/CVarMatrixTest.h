// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imath/CVarMatrix.h>
#include <itest/CStandardTestExecutor.h>

class CVarMatrixTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void SizeConstructorTest();
	void CopyConstructorTest();
	void ClearTest();
	void InitToIdentityTest();
	void GetSetElementTest();
	void GetMinMaxElementTest();
	void AdditionTest();
	void SubtractionTest();
	void NegationTest();
	void MultiplicationTest();
	void ScalarMultiplicationTest();
	void TransposeTest();
	void GetTraceTest();
	void FrobeniusNormTest();
	void ComparisonOperatorsTest();

	void cleanupTestCase();
};


