// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "TPixelConversionTest.h"


void TPixelConversionTest::GrayToGrayConversionTest()
{
	iimg::TPixelConversion<quint8, quint8> converter;
	
	// Test conversion of various gray values
	QCOMPARE(converter(0), quint8(0));
	QCOMPARE(converter(127), quint8(127));
	QCOMPARE(converter(255), quint8(255));
	
	// Test white intensity for gray scale
	QCOMPARE(converter.whiteIntensity, 255.0);
}


void TPixelConversionTest::GrayToFloatConversionTest()
{
	iimg::TPixelConversion<quint8, float, float> converter;
	
	// Test conversion from 8-bit gray to float [0..1]
	float result0 = converter(0);
	QVERIFY(qAbs(result0 - 0.0f) < 0.001f);
	
	float result127 = converter(127);
	QVERIFY(qAbs(result127 - 0.498f) < 0.01f);  // 127/255 ~ 0.498
	
	float result255 = converter(255);
	QVERIFY(qAbs(result255 - 1.0f) < 0.001f);
}


void TPixelConversionTest::FloatToGrayConversionTest()
{
	iimg::TPixelConversion<float, quint8, float> converter;
	
	// Test conversion from float [0..1] to 8-bit gray
	QCOMPARE(converter(0.0f), quint8(0));
	QCOMPARE(converter(0.5f), quint8(127));  // 0.5 * 255 = 127
	QCOMPARE(converter(1.0f), quint8(255));
	
	// Test edge cases
	QCOMPARE(converter(0.0f), quint8(0));
	QCOMPARE(converter(1.0f), quint8(255));
}


void TPixelConversionTest::FloatToFloatConversionTest()
{
	iimg::TPixelConversion<float, float> converter;
	
	// Test direct float to float conversion (should be identity)
	QVERIFY(qAbs(converter(0.0f) - 0.0f) < 0.001f);
	QVERIFY(qAbs(converter(0.5f) - 0.5f) < 0.001f);
	QVERIFY(qAbs(converter(1.0f) - 1.0f) < 0.001f);
	QVERIFY(qAbs(converter(2.5f) - 2.5f) < 0.001f);
}


void TPixelConversionTest::GetWeightTest()
{
	iimg::TPixelConversion<quint8, quint8> converter;
	
	// Default weight should be 1.0
	double weight = converter.GetWeight(128);
	QCOMPARE(weight, 1.0);
	
	// Test with different pixel values
	QCOMPARE(converter.GetWeight(0), 1.0);
	QCOMPARE(converter.GetWeight(255), 1.0);
}


void TPixelConversionTest::GetIntensityTest()
{
	iimg::TPixelConversion<quint8, quint8> converter;
	
	// Intensity should be the pixel value converted to double
	double intensity0 = converter.GetIntensity(0);
	QCOMPARE(intensity0, 0.0);
	
	double intensity127 = converter.GetIntensity(127);
	QCOMPARE(intensity127, 127.0);
	
	double intensity255 = converter.GetIntensity(255);
	QCOMPARE(intensity255, 255.0);
}


void TPixelConversionTest::WhiteIntensityTest()
{
	// Gray to gray should have white intensity of 255
	iimg::TPixelConversion<quint8, quint8> grayConverter;
	QCOMPARE(grayConverter.whiteIntensity, 255.0);
	
	// Float to float should have white intensity of 1
	iimg::TPixelConversion<float, float> floatConverter;
	QCOMPARE(floatConverter.whiteIntensity, 1.0);
}


void TPixelConversionTest::GetCalcTest()
{
	// Test gray to float conversion via GetCalc
	iimg::TPixelConversion<quint8, float, float> converter;
	
	float calc0 = converter.GetCalc(0);
	QVERIFY(qAbs(calc0 - 0.0f) < 0.001f);
	
	float calc255 = converter.GetCalc(255);
	QVERIFY(qAbs(calc255 - 1.0f) < 0.001f);
	
	// Test with intermediate value
	float calc128 = converter.GetCalc(128);
	QVERIFY(qAbs(calc128 - (128.0f / 255.0f)) < 0.01f);
}


void TPixelConversionTest::GetDestTest()
{
	// Test float to gray conversion via GetDest
	iimg::TPixelConversion<float, quint8, float> converter;
	
	quint8 dest0 = converter.GetDest(0.0f);
	QCOMPARE(dest0, quint8(0));
	
	quint8 dest1 = converter.GetDest(1.0f);
	QCOMPARE(dest1, quint8(255));
	
	// Test with intermediate value
	quint8 destHalf = converter.GetDest(0.5f);
	QCOMPARE(destHalf, quint8(127));
}


I_ADD_TEST(TPixelConversionTest);
