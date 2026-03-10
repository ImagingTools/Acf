// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/TArray.h>
#include <itest/CStandardTestExecutor.h>

class TArrayTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void SizeConstructorTest();
	void CopyConstructorTest();
	void IsEmptyTest();
	void GetSetSizesTest();
	void GetSetElementTest();
	void SetAllElementsTest();
	void IteratorTest();

	void cleanupTestCase();
};


