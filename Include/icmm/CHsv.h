#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of HSV (Hue, Saturation, Value) color values.
	
	\section HsvPurpose Purpose
	CHsv represents colors in the HSV color model, which is more intuitive for
	human color selection than RGB. HSV separates color (hue) from intensity
	(value) and colorfulness (saturation), making it easier to create color
	pickers, adjust brightness, or perform color-based image processing.

	\section HsvColorModel HSV Color Model
	HSV consists of three cylindrical coordinates:
	- **Hue (H)**: The color type, represented as an angle [0, 1] where:
		- 0.0 / 1.0 = Red
		- ~0.167 = Yellow  
		- ~0.333 = Green
		- ~0.5 = Cyan
		- ~0.667 = Blue
		- ~0.833 = Magenta
	- **Saturation (S)**: Color intensity [0, 1]:
		- 0.0 = Gray (no color)
		- 1.0 = Pure, vivid color
	- **Value (V)**: Brightness [0, 1]:
		- 0.0 = Black
		- 1.0 = Bright color

	\section HsvUsage Usage Examples
	\code
	// Example 1: Creating HSV colors
	icmm::CHsv red(0.0, 1.0, 1.0);        // Pure red
	icmm::CHsv yellow(0.167, 1.0, 1.0);   // Pure yellow
	icmm::CHsv green(0.333, 1.0, 1.0);    // Pure green
	icmm::CHsv cyan(0.5, 1.0, 1.0);       // Pure cyan
	icmm::CHsv blue(0.667, 1.0, 1.0);     // Pure blue
	icmm::CHsv magenta(0.833, 1.0, 1.0);  // Pure magenta
	
	icmm::CHsv pastelBlue(0.667, 0.3, 0.9);   // Low saturation = pastel
	icmm::CHsv darkRed(0.0, 1.0, 0.3);        // Low value = dark
	icmm::CHsv gray(0.0, 0.0, 0.5);           // Zero saturation = gray

	// Example 2: Accessing and modifying components
	icmm::CHsv color;
	color.SetHue(0.5);          // Cyan hue
	color.SetSaturation(0.7);   // Fairly saturated
	color.SetValue(0.9);        // Bright
	
	double h = color.GetHue();
	double s = color.GetSaturation();
	double v = color.GetValue();

	// Example 3: Adjusting brightness
	icmm::CHsv AdjustBrightness(const icmm::CHsv& color, double factor)
	{
		icmm::CHsv result = color;
		result.SetValue(color.GetValue() * factor);
		// Clamp to [0, 1]
		if (result.GetValue() > 1.0) result.SetValue(1.0);
		if (result.GetValue() < 0.0) result.SetValue(0.0);
		return result;
	}
	
	icmm::CHsv original(0.667, 0.8, 0.6);  // Blue
	icmm::CHsv brighter = AdjustBrightness(original, 1.5);
	icmm::CHsv darker = AdjustBrightness(original, 0.5);

	// Example 4: Adjusting saturation
	icmm::CHsv ChangeSaturation(const icmm::CHsv& color, double newSat)
	{
		icmm::CHsv result = color;
		result.SetSaturation(qBound(0.0, newSat, 1.0));
		return result;
	}
	
	// Make more vivid
	icmm::CHsv vivid = ChangeSaturation(original, 1.0);
	// Make more pastel
	icmm::CHsv pastel = ChangeSaturation(original, 0.3);
	// Make grayscale
	icmm::CHsv gray = ChangeSaturation(original, 0.0);

	// Example 5: Hue rotation (color shift)
	icmm::CHsv RotateHue(const icmm::CHsv& color, double rotation)
	{
		icmm::CHsv result = color;
		double newHue = color.GetHue() + rotation;
		// Wrap around [0, 1)
		while (newHue >= 1.0) newHue -= 1.0;
		while (newHue < 0.0) newHue += 1.0;
		result.SetHue(newHue);
		return result;
	}
	
	icmm::CHsv red(0.0, 1.0, 1.0);
	icmm::CHsv complementary = RotateHue(red, 0.5);  // Cyan (opposite)
	icmm::CHsv analogous = RotateHue(red, 0.083);    // Orange (nearby)

	// Example 6: Creating color harmonies
	QList<icmm::CHsv> CreateTriadicHarmony(const icmm::CHsv& base)
	{
		QList<icmm::CHsv> colors;
		colors.append(base);
		colors.append(RotateHue(base, 1.0 / 3.0));  // 120°
		colors.append(RotateHue(base, 2.0 / 3.0));  // 240°
		return colors;
	}

	// Example 7: RGB to HSV conversion
	icmm::CHsv ConvertRgbToHsv(const icmm::CRgb& rgb)
	{
		icmm::CRgbColorModel rgbModel;
		// Use transformation for conversion
		// (actual implementation uses color transformation)
		// ...
	}

	// Example 8: Color wheel generation
	QList<icmm::CHsv> GenerateColorWheel(int steps)
	{
		QList<icmm::CHsv> colors;
		for (int i = 0; i < steps; ++i) {
			double hue = static_cast<double>(i) / steps;
			colors.append(icmm::CHsv(hue, 1.0, 1.0));
		}
		return colors;
	}

	// Example 9: Creating a color picker
	icmm::CHsv SelectColorFromWheel(double angle,      // 0-360 degrees
	                                double distance,   // 0-1 from center
	                                double brightness) // 0-1
	{
		double hue = angle / 360.0;
		double saturation = distance;
		double value = brightness;
		return icmm::CHsv(hue, saturation, value);
	}
	\endcode

	\section HsvAdvantages HSV Advantages
	- **Intuitive**: Matches human color perception better than RGB
	- **Easy adjustments**: Brightness and saturation can be modified independently
	- **Color selection**: Natural for color pickers (color wheel + brightness slider)
	- **Image processing**: Useful for color-based segmentation and filtering
	- **Color harmonies**: Easy to create complementary, analogous, triadic schemes

	\section HsvVsRgb HSV vs RGB
	- **HSV**: Better for user interfaces and artistic color selection
	- **RGB**: Better for display, calculation, and blending
	- **Conversion**: RGB ↔ HSV transformations are available but have cost
	- **Storage**: Store in the space where most operations will occur

	\section HsvBestPractices Best Practices
	- Use HSV for color pickers and user-facing color selection
	- Convert to RGB for rendering and display
	- Hue is circular - handle wraparound when rotating
	- At V=0 (black) or S=0 (gray), hue is undefined
	- For perceptual operations, consider using Lab color space instead

	\note Component values are normalized to [0, 1] range where:
	      - Hue: 0.0-1.0 represents 0°-360°
	      - Saturation: 0.0 (gray) to 1.0 (pure color)
	      - Value: 0.0 (black) to 1.0 (bright)

	\sa icmm::TComposedColor, icmm::CRgb, icmm::CHsvToRgbTransformation,
	    icmm::CRgbToHsvTranformation

	\ingroup Color
*/
class CHsv: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_HUE,
		CI_SATURATION,
		CI_VALUE,
		CI_LAST = CI_VALUE
	};

	CHsv(double h = 0.0, double s = 0.0, double v = 0.0);
	CHsv(const CHsv& color);
	CHsv(const imath::CVarVector& vector);

	// access to component values
	double GetHue() const;
	void SetHue(double value);
	double GetSaturation() const;
	void SetSaturation(double value);
	double GetValue() const;
	void SetValue(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CHsv operator+(const CHsv& color) const;
	CHsv operator-(const CHsv& color) const;
	CHsv operator*(const CHsv& color) const;
	CHsv operator/(const CHsv& color) const;

	CHsv operator*(double value) const;
	CHsv operator/(double value) const;

	CHsv& operator=(const CHsv& color);

	const CHsv& operator+=(const CHsv& color);
	const CHsv& operator-=(const CHsv& color);
	const CHsv& operator*=(const CHsv& color);
	const CHsv& operator/=(const CHsv& color);

	const CHsv& operator*=(double value);
	const CHsv& operator/=(double value);
};


// inline methods

inline CHsv::CHsv(double h, double s, double v)
{
	SetElement(CI_HUE, h);
	SetElement(CI_SATURATION, s);
	SetElement(CI_VALUE, v);
}


inline CHsv::CHsv(const CHsv& color)
:	BaseClass(color)
{
}


inline CHsv::CHsv(const imath::CVarVector& vector)
:	BaseClass(vector)
{
}


// access to components

inline double CHsv::GetHue() const
{
	return GetElement(CI_HUE);
}


inline void CHsv::SetHue(double value)
{
	SetElement(CI_HUE, value);
}


inline double CHsv::GetSaturation() const
{
	return GetElement(CI_SATURATION);
}


inline void CHsv::SetSaturation(double value)
{
	SetElement(CI_SATURATION, value);
}


inline double CHsv::GetValue() const
{
	return GetElement(CI_VALUE);
}


inline void CHsv::SetValue(double value)
{
	SetElement(CI_VALUE, value);
}


// operators

inline CHsv CHsv::operator+(const CHsv& color) const
{
	CHsv retVal = *this;

	retVal += color;

	return retVal;
}


inline CHsv CHsv::operator-(const CHsv& color) const
{
	CHsv retVal = *this;

	retVal -= color;

	return retVal;
}


inline CHsv CHsv::operator*(const CHsv& color) const
{
	CHsv retVal = *this;

	retVal *= color;

	return retVal;
}


inline CHsv CHsv::operator/(const CHsv& color) const
{
	CHsv retVal = *this;

	retVal /= color;

	return retVal;
}


inline CHsv CHsv::operator*(double value) const
{
	CHsv retVal = *this;

	retVal *= value;

	return retVal;
}


inline CHsv CHsv::operator/(double value) const
{
	CHsv retVal = *this;

	retVal /= value;

	return retVal;
}


inline CHsv& CHsv::operator=(const CHsv& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CHsv& CHsv::operator+=(const CHsv& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CHsv& CHsv::operator-=(const CHsv& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CHsv& CHsv::operator*=(const CHsv& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CHsv& CHsv::operator/=(const CHsv& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CHsv& CHsv::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CHsv& CHsv::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


