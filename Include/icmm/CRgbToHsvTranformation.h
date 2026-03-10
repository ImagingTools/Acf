// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of RGB-to-HSV color transformation.
	
	\section RgbToHsvPurpose Purpose
	CRgbToHsvTranformation converts colors from RGB (Red, Green, Blue) color space
	to HSV (Hue, Saturation, Value) color space. HSV is more intuitive for color
	selection and manipulation, representing colors in terms of their hue (color type),
	saturation (color intensity), and value (brightness).

	\section RgbToHsvAlgorithm Conversion Algorithm
	The RGB to HSV conversion involves:
	1. **Finding min/max**: Determine minimum and maximum RGB components
	2. **Calculate Value**: V = max(R, G, B)
	3. **Calculate Saturation**: S = (max - min) / max (if max != 0)
	4. **Calculate Hue**: Based on which component is maximum:
	   - If R is max: H = (G - B) / (max - min)
	   - If G is max: H = 2 + (B - R) / (max - min)
	   - If B is max: H = 4 + (R - G) / (max - min)
	   - Then: H = H * 60 degrees (and add 360 degrees if negative)

	\section RgbToHsvUsage Usage Examples
	\code
	// Example 1: Basic RGB to HSV conversion
	icmm::CRgbToHsvTranformation rgbToHsv;
	
	// Create RGB color (orange)
	icmm::CVarColor rgb(3);
	rgb.SetElement(0, 1.0);   // Red
	rgb.SetElement(1, 0.5);   // Green
	rgb.SetElement(2, 0.0);   // Blue
	
	// Convert to HSV
	icmm::CVarColor hsv;
	if (rgbToHsv.GetValueAt(rgb, hsv)) {
		double hue = hsv.GetElement(0);         // ~30 degrees (orange hue)
		double saturation = hsv.GetElement(1);  // 1.0 (fully saturated)
		double value = hsv.GetElement(2);       // 1.0 (maximum brightness)
	}

	// Example 2: Converting multiple colors
	QList<icmm::CVarColor> ConvertRgbListToHsv(
		const QList<icmm::CVarColor>& rgbColors)
	{
		icmm::CRgbToHsvTranformation transform;
		QList<icmm::CVarColor> hsvColors;
		
		for (const icmm::CVarColor& rgb : rgbColors) {
			icmm::CVarColor hsv = transform.GetValueAt(rgb);
			hsvColors.append(hsv);
		}
		
		return hsvColors;
	}

	// Example 3: Analyzing color properties via HSV
	struct ColorProperties {
		bool isGrayscale;
		bool isBright;
		double dominantHue;
	};
	
	ColorProperties AnalyzeRgbColor(const icmm::CVarColor& rgb)
	{
		icmm::CRgbToHsvTranformation rgbToHsv;
		icmm::CVarColor hsv = rgbToHsv.GetValueAt(rgb);
		
		ColorProperties props;
		
		// Check if grayscale (low saturation)
		props.isGrayscale = (hsv.GetElement(1) < 0.1);  // Saturation < 10%
		
		// Check if bright
		props.isBright = (hsv.GetElement(2) > 0.5);     // Value > 50%
		
		// Get dominant hue
		props.dominantHue = hsv.GetElement(0);           // Hue in degrees
		
		return props;
	}

	// Example 4: Color picker implementation
	icmm::CVarColor GetColorFromPicker(double hue, double saturation, double value)
	{
		// Create HSV color
		icmm::CVarColor hsv(3);
		hsv.SetElement(0, hue);         // 0-360 degrees
		hsv.SetElement(1, saturation);  // 0-1
		hsv.SetElement(2, value);       // 0-1
		
		// Convert to RGB for rendering
		icmm::CHsvToRgbTransformation hsvToRgb;
		icmm::CVarColor rgb = hsvToRgb.GetValueAt(hsv);
		
		return rgb;
	}

	// Example 5: Detecting dominant colors
	QString GetDominantColorName(const icmm::CVarColor& rgb)
	{
		icmm::CRgbToHsvTranformation rgbToHsv;
		icmm::CVarColor hsv = rgbToHsv.GetValueAt(rgb);
		
		double hue = hsv.GetElement(0);
		double sat = hsv.GetElement(1);
		double val = hsv.GetElement(2);
		
		// Check for grayscale first
		if (sat < 0.1) {
			if (val < 0.3) return "Black";
			if (val > 0.7) return "White";
			return "Gray";
		}
		
		// Determine color from hue
		if (hue < 30 || hue >= 330) return "Red";
		if (hue < 90) return "Yellow";
		if (hue < 150) return "Green";
		if (hue < 210) return "Cyan";
		if (hue < 270) return "Blue";
		return "Magenta";
	}

	// Example 6: Brightness adjustment (via HSV)
	icmm::CVarColor AdjustBrightness(const icmm::CVarColor& rgb, 
	                                 double brightnessFactor)
	{
		// Convert to HSV
		icmm::CRgbToHsvTranformation rgbToHsv;
		icmm::CVarColor hsv = rgbToHsv.GetValueAt(rgb);
		
		// Adjust value (brightness)
		double value = hsv.GetElement(2) * brightnessFactor;
		value = qBound(0.0, value, 1.0);  // Clamp to [0, 1]
		hsv.SetElement(2, value);
		
		// Convert back to RGB
		icmm::CHsvToRgbTransformation hsvToRgb;
		return hsvToRgb.GetValueAt(hsv);
	}

	// Example 7: Saturation boost
	icmm::CVarColor BoostSaturation(const icmm::CVarColor& rgb)
	{
		icmm::CRgbToHsvTranformation rgbToHsv;
		icmm::CVarColor hsv = rgbToHsv.GetValueAt(rgb);
		
		// Increase saturation
		double sat = hsv.GetElement(1) * 1.5;  // 50% increase
		sat = qMin(sat, 1.0);  // Clamp to 1.0
		hsv.SetElement(1, sat);
		
		// Convert back
		icmm::CHsvToRgbTransformation hsvToRgb;
		return hsvToRgb.GetValueAt(hsv);
	}
	\endcode

	\section RgbToHsvApplications Applications
	- **Color pickers**: HSV is more intuitive for UI color selection
	- **Image processing**: Adjusting brightness/saturation independently
	- **Color analysis**: Identifying dominant hues and properties
	- **Color harmonies**: Creating complementary/triadic color schemes
	- **Color correction**: Selective hue adjustments
	- **Computer vision**: Color-based object detection

	\section RgbToHsvRanges Value Ranges
	**Input (RGB):**
	- Red, Green, Blue: [0.0, 1.0]
	
	**Output (HSV):**
	- Hue: [0.0, 360.0] degrees
	- Saturation: [0.0, 1.0]
	- Value: [0.0, 1.0]

	\section RgbToHsvBestPractices Best Practices
	- Use HSV for intuitive color manipulation
	- Convert to HSV, adjust, convert back to RGB for display
	- Check saturation for grayscale detection
	- Use hue ranges for color classification
	- Cache transformation object for batch conversions
	- Remember: HSV is device-dependent (uses RGB as input)

	\note The name contains a typo "Tranformation" instead of "Transformation" but
	      is kept for API compatibility.

	\sa icmm::CHsvToRgbTransformation, icmm::IColorTransformation,
	    icmm::CRgb, icmm::CHsv, icmm::CRgbColorModel

	\ingroup Color
*/
class CRgbToHsvTranformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;
};


} // namespace icmm


