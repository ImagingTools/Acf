// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imath/TMatrix.h>
#include <itest/CStandardTestExecutor.h>

class TMatrixTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void InitModeConstructorTest();
	void CopyConstructorTest();
	void ClearResetTest();
	void GetSetElementTest();
	void InitToIdentityTest();
	void GetMinMaxElementTest();
	void NegationTest();
	void AdditionTest();
	void SubtractionTest();
	void MatrixMultiplicationTest();
	void VectorMultiplicationTest();
	void ScalarMultiplicationTest();
	void TransposeTest();
	void GetTraceTest();
	void FrobeniusNormTest();
	void GetColumnRowVectorTest();
	void SetColumnRowVectorTest();

	void cleanupTestCase();
};


