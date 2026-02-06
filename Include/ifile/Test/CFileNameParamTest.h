// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <ifile/CFileNameParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <itest/CStandardTestExecutor.h>


class CFileNameParamTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void GetSetPathTest();
	void PathTypeTest();
	void SerializationTest();
	void CopyFromTest();
	void IsEqualTest();
	void EmptyPathTest();
	void SupportedOperationsTest();
};


