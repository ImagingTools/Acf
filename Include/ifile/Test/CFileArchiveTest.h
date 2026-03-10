// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CFileArchiveTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void BasicSerializationTest();
	void PrimitiveTypesTest();
	void StringSerializationTest();
	void TagSkippingTest();
	void InvalidFilePathTest();
	void MultipleObjectsTest();
};


