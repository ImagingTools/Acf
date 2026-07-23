// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CComposedColorTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	// CRgb
	void RgbConstructorTest();
	void RgbGetSetTest();
	void RgbOperatorsTest();
	void RgbNormalizeTest();
	void RgbSerializeEdgeCasesTest();

	// CCmy
	void CmyConstructorTest();
	void CmyGetSetTest();
	void CmyOperatorsTest();

	// CCmyk
	void CmykConstructorTest();
	void CmykGetSetTest();
	void CmykOperatorsTest();

	// CLab
	void LabConstructorTest();
	void LabGetSetTest();
	void LabDeltaETest();
	void LabSerializeEdgeCasesTest();

	// common behavior
	void IsSimilarTest();
	void AssignScalarTest();
	void VarVectorConversionTest();
};
