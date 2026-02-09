// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CFileSecureArchiveTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void BasicSecureSerializationTest();
	void PrimitiveTypesSecureTest();
	void StringSecureSerializationTest();
	void SecureArchiveIntegrityTest();
	void CompareSecureVsNormalTest();
	void MultipleObjectsSecureTest();
};


