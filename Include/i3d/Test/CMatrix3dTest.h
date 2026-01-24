// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <i3d/CMatrix3d.h>
#include <itest/CStandardTestExecutor.h>

class CMatrix3dTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void ResetTest();
	void GetIdentityTest();
	void GetMultipliedVectorTest();
	void GetMultipliedMatrixTest();
	void GetAxisTest();
	void GetTransposedTest();
	void GetDeterminantTest();
	void ScalarOperatorsTest();

	void cleanupTestCase();
};


