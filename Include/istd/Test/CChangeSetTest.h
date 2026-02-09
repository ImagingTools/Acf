// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/IChangeable.h>
#include <itest/CStandardTestExecutor.h>

class CChangeSetTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void DefaultConstructorTest();
	void SingleIdConstructorTest();
	void MultipleIdsConstructorTest();
	void DescriptionConstructorTest();
	void IsEmptyTest();
	void ContainsTest();
	void ContainsAnyTest();
	void GetIdsTest();
	void ResetTest();
	void MaskOutTest();

	void cleanupTestCase();
};


