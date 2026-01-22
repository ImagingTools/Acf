// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iprm/CTextParam.h>


class CTextParamTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void GetSetTest();
	void SetSameTextTest();
	void SetEmptyTextTest();
	void GetSupportedOperationsTest();
	void ResetTest();
	void ResetEmptyTest();
	void SerializeTest();
	void CopyFromTest();
	void CopyFromIncompatibleTest();
	void CloneTest();
	void CloneWithModeTest();
	void IsEqualTest();
	void IsEqualDifferentTextTest();
	void IsEqualIncompatibleTest();
	void IsReadOnlyTest();

	void cleanupTestCase();

private:
	iprm::CTextParam m_param;
	const QString m_text = "Test Text Value";
};


