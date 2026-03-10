// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iimg/CPixelFormatList.h>
#include <iimg/IBitmap.h>
#include <itest/CStandardTestExecutor.h>

class CPixelFormatListTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void GetInstanceTest();
	void GetOptionsCountTest();
	void GetOptionNameTest();
	void GetOptionDescriptionTest();
	void GetOptionIdTest();
	void IsOptionEnabledTest();
	void GetActiveFormatsTest();
	void SetActiveFormatsTest();
	void GetOptionsFlagsTest();

	void cleanupTestCase();

private:
	iimg::CPixelFormatList* m_formatListPtr = nullptr;
};
