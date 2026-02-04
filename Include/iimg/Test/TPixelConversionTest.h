// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iimg/TPixelConversion.h>
#include <itest/CStandardTestExecutor.h>

class TPixelConversionTest: public QObject
{
	Q_OBJECT
private slots:
	void GrayToGrayConversionTest();
	void GrayToFloatConversionTest();
	void FloatToGrayConversionTest();
	void FloatToFloatConversionTest();
	void GetWeightTest();
	void GetIntensityTest();
	void WhiteIntensityTest();
	void GetCalcTest();
	void GetDestTest();
};
