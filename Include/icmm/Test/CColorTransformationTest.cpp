// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CColorTransformationTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <icmm/CCmyk.h>
#include <icmm/CCmykToRgbTransformation.h>
#include <icmm/CHsv.h>
#include <icmm/CHsvToRgbTransformation.h>
#include <icmm/CLab.h>
#include <icmm/CRgb.h>
#include <icmm/CRgbToCmykTransformation.h>
#include <icmm/CRgbToHsvTranformation.h>
#include <icmm/CRgbToXyzTransformation.h>
#include <icmm/CXyzToCieLabTransformation.h>
#include <icmm/TColorGradient.h>


namespace
{

icmm::CVarColor MakeColor(std::initializer_list<double> values)
{
	icmm::CVarColor color(int(values.size()));

	int index = 0;
	for (double value: values){
		color.SetElement(index++, value);
	}

	return color;
}

}


void CColorTransformationTest::RgbToHsvTest()
{
	icmm::CRgbToHsvTranformation transformation;

	// Pure red: hue 0, full saturation, full value
	icmm::CVarColor hsv(3);
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 0.0, 0.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_HUE)) < 0.001);
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_SATURATION) - 1.0) < 0.001);
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_VALUE) - 1.0) < 0.001);

	// Pure green: hue 120 degrees
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 1.0, 0.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_HUE) - 120.0) < 0.001);

	// Pure blue: hue 240 degrees
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 1.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_HUE) - 240.0) < 0.001);

	// Yellow: hue 60 degrees
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 1.0, 0.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_HUE) - 60.0) < 0.001);

	// 50% saturation
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 0.5, 0.5}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_SATURATION) - 0.5) < 0.001);
}


void CColorTransformationTest::RgbToHsvEdgeCasesTest()
{
	icmm::CRgbToHsvTranformation transformation;
	icmm::CVarColor hsv(3);

	// Achromatic gray: saturation and hue are 0
	QVERIFY(transformation.GetValueAt(MakeColor({0.5, 0.5, 0.5}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_HUE)) < 0.001);
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_SATURATION)) < 0.001);
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_VALUE) - 0.5) < 0.001);

	// Black
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_VALUE)) < 0.001);
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_SATURATION)) < 0.001);

	// White
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 1.0, 1.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_VALUE) - 1.0) < 0.001);
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_SATURATION)) < 0.001);

	// Magenta-like color produces a hue above 240 degrees (negative hue branch)
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 0.0, 1.0}), hsv));
	QVERIFY(qAbs(hsv.GetElement(icmm::CHsv::CI_HUE) - 300.0) < 0.001);
}


void CColorTransformationTest::HsvToRgbTest()
{
	icmm::CHsvToRgbTransformation transformation;
	icmm::CVarColor rgb(3);

	// Achromatic input: value copied into all channels
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.75}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED) - 0.75) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN) - 0.75) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE) - 0.75) < 0.001);

	// Pure red (hue is normalized to [0, 1])
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 1.0, 1.0}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED) - 1.0) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN)) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE)) < 0.001);

	// Pure green (hue 120/360)
	QVERIFY(transformation.GetValueAt(MakeColor({120.0 / 360.0, 1.0, 1.0}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED)) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN) - 1.0) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE)) < 0.001);

	// Hue 1.0 (360 degrees) behaves like hue 0.0
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 1.0, 1.0}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED) - 1.0) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN)) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE)) < 0.001);
}


void CColorTransformationTest::RgbHsvRoundTripTest()
{
	icmm::CRgbToHsvTranformation rgbToHsv;
	icmm::CHsvToRgbTransformation hsvToRgb;

	const QList<icmm::CVarColor> testColors = {
		MakeColor({1.0, 0.0, 0.0}),   // red
		MakeColor({0.0, 1.0, 0.0}),   // green
		MakeColor({0.0, 0.0, 1.0}),   // blue
		MakeColor({1.0, 1.0, 0.0}),   // yellow
		MakeColor({0.0, 1.0, 1.0}),   // cyan
		MakeColor({0.5, 0.5, 0.5}),   // gray
		MakeColor({0.3, 0.6, 0.9})};  // arbitrary color

	for (const icmm::CVarColor& rgb: testColors){
		icmm::CVarColor hsv(3);
		QVERIFY(rgbToHsv.GetValueAt(rgb, hsv));

		// RGB->HSV returns hue in degrees, HSV->RGB expects hue in [0, 1]
		hsv.SetElement(icmm::CHsv::CI_HUE, hsv.GetElement(icmm::CHsv::CI_HUE) / 360.0);

		icmm::CVarColor restored(3);
		QVERIFY(hsvToRgb.GetValueAt(hsv, restored));
		QVERIFY(restored.IsSimilar(rgb, 0.001));
	}
}


void CColorTransformationTest::RgbToCmykTest()
{
	icmm::CRgbToCmykTransformation transformation;
	icmm::CVarColor cmyk(4);

	// White: no ink
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 1.0, 1.0}), cmyk));
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_CYAN)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_MAGENTA)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_YELLOW)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_BLACK)) < 0.001);

	// Black: full black ink only
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.0}), cmyk));
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_CYAN)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_MAGENTA)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_YELLOW)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_BLACK) - 1.0) < 0.001);

	// Pure red: full magenta and yellow, no black
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 0.0, 0.0}), cmyk));
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_CYAN)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_MAGENTA) - 1.0) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_YELLOW) - 1.0) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_BLACK)) < 0.001);

	// 50% gray: only black ink is used (under color removal)
	QVERIFY(transformation.GetValueAt(MakeColor({0.5, 0.5, 0.5}), cmyk));
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_CYAN)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_MAGENTA)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_YELLOW)) < 0.001);
	QVERIFY(qAbs(cmyk.GetElement(icmm::CCmyk::CI_BLACK) - 0.5) < 0.001);

	// Single argument overload returns a 4-component color
	icmm::CVarColor result = transformation.GetValueAt(MakeColor({1.0, 1.0, 1.0}));
	QCOMPARE(result.GetElementsCount(), 4);
}


void CColorTransformationTest::CmykToRgbTest()
{
	icmm::CCmykToRgbTransformation transformation;
	icmm::CVarColor rgb(3);

	// No ink: white
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.0, 0.0}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED) - 1.0) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN) - 1.0) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE) - 1.0) < 0.001);

	// Full black ink: black
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.0, 1.0}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED)) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN)) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE)) < 0.001);

	// Full cyan: no red
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 0.0, 0.0, 0.0}), rgb));
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_RED)) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_GREEN) - 1.0) < 0.001);
	QVERIFY(qAbs(rgb.GetElement(icmm::CRgb::CI_BLUE) - 1.0) < 0.001);
}


void CColorTransformationTest::RgbCmykRoundTripTest()
{
	icmm::CRgbToCmykTransformation rgbToCmyk;
	icmm::CCmykToRgbTransformation cmykToRgb;

	const QList<icmm::CVarColor> testColors = {
		MakeColor({1.0, 1.0, 1.0}),   // white
		MakeColor({0.0, 0.0, 0.0}),   // black
		MakeColor({1.0, 0.0, 0.0}),   // red
		MakeColor({0.0, 1.0, 0.0}),   // green
		MakeColor({0.0, 0.0, 1.0}),   // blue
		MakeColor({0.5, 0.5, 0.5}),   // gray
		MakeColor({0.3, 0.6, 0.9})};  // arbitrary color

	for (const icmm::CVarColor& rgb: testColors){
		icmm::CVarColor cmyk(4);
		QVERIFY(rgbToCmyk.GetValueAt(rgb, cmyk));

		icmm::CVarColor restored(3);
		QVERIFY(cmykToRgb.GetValueAt(cmyk, restored));
		QVERIFY(restored.IsSimilar(rgb, 0.001));
	}
}


void CColorTransformationTest::RgbToXyzTest()
{
	icmm::CRgbToXyzTransformation transformation;
	icmm::CVarColor xyz(3);

	// sRGB white maps to the D65 white point (scaled to [0, 1])
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 1.0, 1.0}), xyz));
	QVERIFY(qAbs(xyz.GetElement(0) - 0.9505) < 0.001);
	QVERIFY(qAbs(xyz.GetElement(1) - 1.0) < 0.001);
	QVERIFY(qAbs(xyz.GetElement(2) - 1.089) < 0.001);

	// Black maps to zero
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.0}), xyz));
	QVERIFY(qAbs(xyz.GetElement(0)) < 0.001);
	QVERIFY(qAbs(xyz.GetElement(1)) < 0.001);
	QVERIFY(qAbs(xyz.GetElement(2)) < 0.001);

	// Luminance of pure green is the largest of the primaries
	icmm::CVarColor redXyz(3);
	icmm::CVarColor greenXyz(3);
	icmm::CVarColor blueXyz(3);
	QVERIFY(transformation.GetValueAt(MakeColor({1.0, 0.0, 0.0}), redXyz));
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 1.0, 0.0}), greenXyz));
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 1.0}), blueXyz));
	QVERIFY(greenXyz.GetElement(1) > redXyz.GetElement(1));
	QVERIFY(redXyz.GetElement(1) > blueXyz.GetElement(1));
}


void CColorTransformationTest::XyzToCieLabTest()
{
	icmm::CXyzToCieLabTransformation transformation;
	icmm::CVarColor lab(3);

	// Reference white (D65, 0..100 scale) maps to L=100, a=0, b=0
	QVERIFY(transformation.GetValueAt(MakeColor({95.047, 100.0, 108.883}), lab));
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_L) - 100.0) < 0.01);
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_A)) < 0.01);
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_B)) < 0.01);

	// Black maps to L=0, a=0, b=0 (linear part of the transfer function)
	QVERIFY(transformation.GetValueAt(MakeColor({0.0, 0.0, 0.0}), lab));
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_L)) < 0.01);
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_A)) < 0.01);
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_B)) < 0.01);

	// 50% luminance gray is neutral
	QVERIFY(transformation.GetValueAt(MakeColor({0.5 * 95.047, 50.0, 0.5 * 108.883}), lab));
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_A)) < 0.01);
	QVERIFY(qAbs(lab.GetElement(icmm::CLab::CI_B)) < 0.01);
	QVERIFY(lab.GetElement(icmm::CLab::CI_L) > 0.0);
	QVERIFY(lab.GetElement(icmm::CLab::CI_L) < 100.0);
}


void CColorTransformationTest::InvalidInputSizeTest()
{
	icmm::CVarColor color2 = MakeColor({0.5, 0.5});
	icmm::CVarColor color3 = MakeColor({0.5, 0.5, 0.5});
	icmm::CVarColor color4 = MakeColor({0.5, 0.5, 0.5, 0.5});

	icmm::CVarColor result3(3);
	icmm::CVarColor result4(4);

	// Wrong argument size is rejected
	QVERIFY(!icmm::CRgbToHsvTranformation().GetValueAt(color4, result3));
	QVERIFY(!icmm::CHsvToRgbTransformation().GetValueAt(color2, result3));
	QVERIFY(!icmm::CRgbToCmykTransformation().GetValueAt(color4, result4));
	QVERIFY(!icmm::CCmykToRgbTransformation().GetValueAt(color3, result3));
	QVERIFY(!icmm::CRgbToXyzTransformation().GetValueAt(color2, result3));
	QVERIFY(!icmm::CXyzToCieLabTransformation().GetValueAt(color4, result3));

	// Wrong result size is rejected
	QVERIFY(!icmm::CRgbToHsvTranformation().GetValueAt(color3, result4));
	QVERIFY(!icmm::CRgbToCmykTransformation().GetValueAt(color3, result3));
	QVERIFY(!icmm::CCmykToRgbTransformation().GetValueAt(color4, result4));
}


void CColorTransformationTest::ColorGradientTest()
{
	icmm::CVarColor startColor = MakeColor({0.0, 0.0, 0.0});
	icmm::CVarColor endColor = MakeColor({1.0, 0.5, 0.0});

	icmm::CLinearColorGradient gradient(startColor, endColor);

	icmm::CVarColor parameter(1);
	icmm::CVarColor result(3);

	// Start of the gradient
	parameter.SetElement(0, 0.0);
	QVERIFY(gradient.GetValueAt(parameter, result));
	QVERIFY(result.IsSimilar(startColor, 0.001));

	// End of the gradient
	parameter.SetElement(0, 1.0);
	QVERIFY(gradient.GetValueAt(parameter, result));
	QVERIFY(result.IsSimilar(endColor, 0.001));

	// Middle of the gradient
	parameter.SetElement(0, 0.5);
	QVERIFY(gradient.GetValueAt(parameter, result));
	QVERIFY(qAbs(result.GetElement(0) - 0.5) < 0.001);
	QVERIFY(qAbs(result.GetElement(1) - 0.25) < 0.001);
	QVERIFY(qAbs(result.GetElement(2)) < 0.001);

	// Parameter values outside [0, 1] are clipped
	parameter.SetElement(0, 2.0);
	QVERIFY(gradient.GetValueAt(parameter, result));
	QVERIFY(result.IsSimilar(endColor, 0.001));

	parameter.SetElement(0, -1.0);
	QVERIFY(gradient.GetValueAt(parameter, result));
	QVERIFY(result.IsSimilar(startColor, 0.001));

	// Single argument overload uses the component count of the start color
	parameter.SetElement(0, 1.0);
	icmm::CVarColor endResult = gradient.GetValueAt(parameter);
	QCOMPARE(endResult.GetElementsCount(), 3);
	QVERIFY(endResult.IsSimilar(endColor, 0.001));
}


I_ADD_TEST(CColorTransformationTest);
