// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CSpectrumTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	// CSpectrumInfo
	void SpectrumInfoConstructorTest();
	void SpectrumInfoSetRangeTest();
	void SpectrumInfoSetStepTest();
	void SpectrumInfoSamplesCountTest();
	void SpectrumInfoSerializeTest();
	void SpectrumInfoChangeableTest();

	// CSpectrum
	void SpectrumConstructorTest();
	void SpectrumInvalidConstructionTest();
	void SpectrumSampleAccessTest();
	void SpectrumSerializeTest();
	void SpectrumChangeableTest();
};
