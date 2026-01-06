#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of RGB color values.
	
	\section RgbPurpose Purpose
	CRgb represents a color in the RGB (Red, Green, Blue) additive color model.
	This is the standard color representation for displays, digital images, and
	light-based color mixing. Each component typically ranges from 0.0 (no intensity)
	to 1.0 (full intensity), though values outside this range are permitted for
	HDR (High Dynamic Range) applications.

	\section RgbColorModel RGB Color Model
	RGB is an additive color model where colors are created by adding together
	red, green, and blue light:
	- **Red + Green = Yellow**
	- **Red + Blue = Magenta**
	- **Green + Blue = Cyan**
	- **Red + Green + Blue = White**
	- **No light (0,0,0) = Black**

	\section RgbUsage Usage Examples
	\code
	// Example 1: Creating RGB colors
	icmm::CRgb black(0.0, 0.0, 0.0);
	icmm::CRgb white(1.0, 1.0, 1.0);
	icmm::CRgb red(1.0, 0.0, 0.0);
	icmm::CRgb green(0.0, 1.0, 0.0);
	icmm::CRgb blue(0.0, 0.0, 1.0);
	icmm::CRgb yellow(1.0, 1.0, 0.0);
	icmm::CRgb magenta(1.0, 0.0, 1.0);
	icmm::CRgb cyan(0.0, 1.0, 1.0);
	icmm::CRgb gray(0.5, 0.5, 0.5);

	// Example 2: Accessing and modifying components
	icmm::CRgb color;
	color.SetRed(0.8);
	color.SetGreen(0.3);
	color.SetBlue(0.2);
	
	double r = color.GetRed();
	double g = color.GetGreen();
	double b = color.GetBlue();

	// Example 3: Color arithmetic
	icmm::CRgb color1(0.8, 0.3, 0.2);
	icmm::CRgb color2(0.2, 0.5, 0.7);
	
	// Add colors (lighten)
	icmm::CRgb lighter = color1 + color2;  // {1.0, 0.8, 0.9}
	
	// Multiply colors (darken/tint)
	icmm::CRgb tinted = color1 * color2;   // {0.16, 0.15, 0.14}
	
	// Scale color (adjust brightness)
	icmm::CRgb brighter = color1 * 1.5;    // {1.2, 0.45, 0.3} (HDR)
	icmm::CRgb dimmer = color1 * 0.5;      // {0.4, 0.15, 0.1}

	// Example 4: Color blending
	icmm::CRgb BlendColors(const icmm::CRgb& a, 
	                       const icmm::CRgb& b, 
	                       double t)  // t in [0, 1]
	{
		// Linear interpolation
		return a * (1.0 - t) + b * t;
	}
	
	icmm::CRgb startColor(1.0, 0.0, 0.0);  // Red
	icmm::CRgb endColor(0.0, 0.0, 1.0);    // Blue
	icmm::CRgb midColor = BlendColors(startColor, endColor, 0.5);  // Purple

	// Example 5: Grayscale conversion
	double ToGrayscale(const icmm::CRgb& color)
	{
		// Perceptual grayscale (ITU-R BT.709)
		return 0.2126 * color.GetRed() + 
		       0.7152 * color.GetGreen() + 
		       0.0722 * color.GetBlue();
	}

	// Example 6: Color clamping
	icmm::CRgb ClampToValidRange(const icmm::CRgb& color)
	{
		icmm::CRgb result = color;
		result.SetRed(qBound(0.0, color.GetRed(), 1.0));
		result.SetGreen(qBound(0.0, color.GetGreen(), 1.0));
		result.SetBlue(qBound(0.0, color.GetBlue(), 1.0));
		return result;
	}

	// Example 7: Serialization
	void SaveColor(const icmm::CRgb& color, const QString& filename)
	{
		ifile::CFileWriteArchive archive(filename);
		if (archive.IsOpen()) {
			icmm::CRgb copy = color;
			copy.Serialize(archive);
		}
	}

	void LoadColor(icmm::CRgb& color, const QString& filename)
	{
		ifile::CFileReadArchive archive(filename);
		if (archive.IsOpen()) {
			color.Serialize(archive);
		}
	}

	// Example 8: Gamma correction
	icmm::CRgb ApplyGamma(const icmm::CRgb& linear, double gamma)
	{
		return icmm::CRgb(
			qPow(linear.GetRed(), 1.0 / gamma),
			qPow(linear.GetGreen(), 1.0 / gamma),
			qPow(linear.GetBlue(), 1.0 / gamma)
		);
	}

	icmm::CRgb RemoveGamma(const icmm::CRgb& srgb, double gamma)
	{
		return icmm::CRgb(
			qPow(srgb.GetRed(), gamma),
			qPow(srgb.GetGreen(), gamma),
			qPow(srgb.GetBlue(), gamma)
		);
	}
	\endcode

	\section RgbComponentRange Component Range
	- **Standard Range**: [0.0, 1.0] for SDR (Standard Dynamic Range)
		- 0.0 = no intensity (black)
		- 1.0 = full intensity (white or pure color)
	- **Extended Range**: Values > 1.0 are valid for HDR applications
	- **Negative Values**: Generally invalid but not prevented by the class

	\section RgbOperators Supported Operations
	- **Arithmetic**: +, -, *, / (component-wise)
	- **Scaling**: *, / with scalar values
	- **Assignment**: =, +=, -=, *=, /=
	- **Serialization**: Save/load via IArchive

	\section RgbBestPractices Best Practices
	- Use [0.0, 1.0] range for standard applications
	- Clamp values before display to avoid artifacts
	- Consider gamma correction for perceptual operations
	- Use color spaces like Lab for perceptual blending
	- Be aware RGB is device-dependent without color profile

	\note Component values are stored as doubles and typically range from 0.0 to 1.0,
	      though values outside this range are permitted.

	\sa icmm::TComposedColor, icmm::CRgbColorModel, icmm::CHsv, icmm::CSRgbColor

	\ingroup Color
*/
class CRgb: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_RED,
		CI_GREEN,
		CI_BLUE,
		CI_LAST = CI_BLUE
	};

	CRgb(double r = 0.0, double g = 0.0, double b = 0.0);
	CRgb(const CRgb& color);

	// access to component values
	double GetRed() const;
	void SetRed(double value);
	double GetGreen() const;
	void SetGreen(double value);
	double GetBlue() const;
	void SetBlue(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CRgb operator+(const CRgb& color) const;
	CRgb operator-(const CRgb& color) const;
	CRgb operator*(const CRgb& color) const;
	CRgb operator/(const CRgb& color) const;

	CRgb operator*(double value) const;
	CRgb operator/(double value) const;

	CRgb& operator=(const CRgb& color);

	const CRgb& operator+=(const CRgb& color);
	const CRgb& operator-=(const CRgb& color);
	const CRgb& operator*=(const CRgb& color);
	const CRgb& operator/=(const CRgb& color);

	const CRgb& operator*=(double value);
	const CRgb& operator/=(double value);
};


// inline methods

inline CRgb::CRgb(double r, double g, double b)
{
	SetElement(CI_RED, r);
	SetElement(CI_GREEN, g);
	SetElement(CI_BLUE, b);
}


inline CRgb::CRgb(const CRgb& color)
:	BaseClass(color)
{
}


// access to components

inline double CRgb::GetRed() const
{
	return GetElement(CI_RED);
}


inline void CRgb::SetRed(double value)
{
	SetElement(CI_RED, value);
}


inline double CRgb::GetGreen() const
{
	return GetElement(CI_GREEN);
}


inline void CRgb::SetGreen(double value)
{
	SetElement(CI_GREEN, value);
}


inline double CRgb::GetBlue() const
{
	return GetElement(CI_BLUE);
}


inline void CRgb::SetBlue(double value)
{
	SetElement(CI_BLUE, value);
}


// operators

inline CRgb CRgb::operator+(const CRgb& color) const
{
	CRgb retVal = *this;

	retVal += color;

	return retVal;
}


inline CRgb CRgb::operator-(const CRgb& color) const
{
	CRgb retVal = *this;

	retVal -= color;

	return retVal;
}


inline CRgb CRgb::operator*(const CRgb& color) const
{
	CRgb retVal = *this;

	retVal *= color;

	return retVal;
}


inline CRgb CRgb::operator/(const CRgb& color) const
{
	CRgb retVal = *this;

	retVal /= color;

	return retVal;
}


inline CRgb CRgb::operator*(double value) const
{
	CRgb retVal = *this;

	retVal *= value;

	return retVal;
}


inline CRgb CRgb::operator/(double value) const
{
	CRgb retVal = *this;

	retVal /= value;

	return retVal;
}


inline CRgb& CRgb::operator=(const CRgb& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CRgb& CRgb::operator+=(const CRgb& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CRgb& CRgb::operator-=(const CRgb& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CRgb& CRgb::operator*=(const CRgb& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CRgb& CRgb::operator/=(const CRgb& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CRgb& CRgb::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CRgb& CRgb::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


